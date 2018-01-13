#include <mbed.h>
#include <mbed_events.h>
#include <InterruptIn.h>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"
#include "Classes/LinearScalable.h"
#include "Classes/ExpScalable.h"
#include "Classes/SerialPrinter.h"
#include "Classes/HumiditySensor.h"
#include "Classes/RTC.h"
#include "Classes/FuelCell.h"

// Defs
#include "Def/constants.h"
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"
#include "Def/semaphore_def.h"
#include "Def/ds3231.h"
#include "Def/SHT31.h"

// Thread src
#include "controller_event_queue.h"
#include "monitoring.h"
#include "error_event_queue.h"
#include "datalink.h"
#include "main.h"
/*
Initilaize Objects
  -These are used by the Threads
    -want the I/O to be global, local objects are defined locally
*/
// Interrupt Objects
InterruptIn h2(H2_OK);
InterruptIn stop(STOP);
InterruptIn err(ERROR_ISR);
InterruptIn estop1(ESTOP1);
InterruptIn estop2(ESTOP2);
//I2C Objects
I2C master(I2C_SDA, I2C_SCL);
HumiditySensor humidity("Humidity", &master);

// Scale objects
LinearScalable<float> cap_scale(1.0, 0.0);

// AnalogIn_Ext Objects
Analog_Sensor<LinearScalable<float> > capvolt(CAPVOLT, cap_scale, "capvolt");
Analog_Sensor<LinearScalable<float> > fccurr(FCCURR, cap_scale, "fccurr");
Analog_Sensor<LinearScalable<float> > fcvolt(FCVOLT, cap_scale, "fcvolt");
Analog_Sensor<LinearScalable<float> > capcurr(CAPCURR, cap_scale, "capcurr");
Analog_Sensor<LinearScalable<float> > motorvolt(MOTORVOLT, cap_scale, "motorvolt");
Analog_Sensor<LinearScalable<float> > motorcurr(MOTORCURR, cap_scale, "motorcurr");
Analog_Sensor<LinearScalable<float> > press1(PRESS1, cap_scale, "press1");
Analog_Sensor<LinearScalable<float> > press2(PRESS2, cap_scale, "press2");
Analog_Sensor<LinearScalable<float> > press3(PRESS3, cap_scale, "press3");
Analog_Sensor<LinearScalable<float> > press4(PRESS4, cap_scale, "press4");
Analog_Sensor<LinearScalable<float> > fctemp1(FCTEMP1, cap_scale, "fctemp1");
Analog_Sensor<LinearScalable<float> > fctemp2(FCTEMP2, cap_scale, "fctemp2");
//Analog_Sensor<LinearScalable<float> > temp1(TEMP1, cap_scale, "temp1");
//Analog_Sensor<LinearScalable<float> > temp2(TEMP2, cap_scale, "temp2");
//Analog_Sensor<LinearScalable<float> > temp3(TEMP3, cap_scale, "temp3");
//Analog_Sensor<LinearScalable<float> > temp4(TEMP4, cap_scale, "temp4");
//Analog_Sensor<LinearScalable<float> > temp5(TEMP5, cap_scale, "temp5");


// DigitalOut_Ext objects
DigitalOut_Ext supply_v(SUPPLY_V, "Supply_V");
DigitalOut_Ext purge_v(PURGE_V, "PURGE_V");
DigitalOut_Ext other1_v(VALVE3, "VALVE3");
DigitalOut_Ext other2_v(VALVE4, "VALVE4");
DigitalOut_Ext start_r(START_R, "START_R");
DigitalOut_Ext motor_r(MOTOR_R, "MOTOR_R");
DigitalOut_Ext charge_r(CHARGE_R, "CHARGE_R");
DigitalOut_Ext cap_r(CAP_R, "CAP_R");
DigitalOut_Ext fcc_r(FCC_R, "FCC_R");
DigitalOut_Ext error_throw(ERROR_ISR_THROW, "ERROR_ISR_THROW");
DigitalOut_Ext alarm_led(ALARM_LED, "ALARM_LED");
DigitalOut_Ext debug_led(DEBUG_LED, "DEBUG_LED");
DigitalOut_Ext shut_led(SHUT_LED, "SHUT_LED");
DigitalOut_Ext run_led(RUN_LED, "RUN_LED");
DigitalOut_Ext start_led(START_LED, "START_LED");
DigitalOut_Ext ol_rst(OL_RST, "OL_RST");
DigitalOut_Ext hum_rst(HUM_RST, "HUM_RST");

// Fan objects
Fan fan1("fan1",PWM_1,TACH_1);
Fan fan2("fan2",PWM_2,TACH_2);
Fan fan3("fan3",PWM_3,TACH_3);

// Integrator objects
Integrator fc_coulumbs("fc_coulumbs");
Integrator fc_joules("fc_joul");
Integrator cap_coulumbs("cap_coulumbs");
Integrator cap_joules("cap_joul");

FuelCell fc("Fuel Cell");

//Vectors
vector<Sensor*> sensor_vec;
vector<Integrator*> int_vec;
vector<DigitalOut_Ext*> dig_out_vec;

//Iterators
vector<Sensor*>::iterator sensor_iter;
vector<Integrator*>::iterator int_iter;
vector<DigitalOut_Ext*>::iterator dig_out_iter;


// Initilaize threads
Thread controller_event_thread;
Thread error_event_thread;
Thread data_event_thread;
Thread FTDI_event_thread;
Thread monitor;

void error_isr(){
  cont_queue.break_dispatch();
  controller_event_thread.terminate();
  error_cleanup();
}

int main() {
  // Attach Interrupts
  //error_throw.write(true);
  h2.rise(&error_isr); // Works
  //err.rise(&error_isr); // Don't work? EDIT: Found out Nucleo only supports interrupts on one port
  h2.mode(PullDown);
  //err.mode(PullDown);
  estop1.rise(&error_isr); // Works
  estop1.mode(PullDown);
  estop2.rise(&error_isr); // Works
  estop2.mode(PullDown);

  //Populate vectors
  sensor_vec.push_back(&fccurr);
  sensor_vec.push_back(&capvolt);
  sensor_vec.push_back(&fcvolt);
  sensor_vec.push_back(&capcurr);
  sensor_vec.push_back(&motorvolt);
  sensor_vec.push_back(&motorcurr);
  sensor_vec.push_back(&press1);
  sensor_vec.push_back(&press2);
  sensor_vec.push_back(&press3);
  sensor_vec.push_back(&press4);
  sensor_vec.push_back(&fctemp1);
  sensor_vec.push_back(&fctemp2);
  //sensor_vec.push_back(&temp1);
  //sensor_vec.push_back(&temp2);
  //sensor_vec.push_back(&temp3);
  //sensor_vec.push_back(&temp4);
  //sensor_vec.push_back(&temp5);

  int_vec.push_back(&fc_coulumbs);
  int_vec.push_back(&fc_joules);
  int_vec.push_back(&cap_coulumbs);
  int_vec.push_back(&cap_joules);

  dig_out_vec.push_back(&supply_v);
  dig_out_vec.push_back(&purge_v);
  dig_out_vec.push_back(&other1_v);
  dig_out_vec.push_back(&other2_v);
  dig_out_vec.push_back(&start_r);
  dig_out_vec.push_back(&motor_r);
  dig_out_vec.push_back(&charge_r);
  dig_out_vec.push_back(&cap_r);
  dig_out_vec.push_back(&fcc_r);
  dig_out_vec.push_back(&error_throw);

  fc_coulumbs.sensor_add(&fccurr);
  fc_joules.sensor_add(&fccurr);
  fc_joules.sensor_add(&fcvolt);
  cap_coulumbs.sensor_add(&capcurr);
  cap_joules.sensor_add(&capcurr);
  cap_joules.sensor_add(&capvolt);

  // Threads from lowest -> highest priority
  monitor.set_priority(osPriorityIdle); // Will be running 90% of the time, since other threads are quick
  data_event_thread.set_priority(osPriorityLow);
  controller_event_thread.set_priority(osPriorityBelowNormal);
  FTDI_event_thread.set_priority(osPriorityNormal);
  error_event_thread.set_priority(osPriorityHigh);

  //Start threads
  error_event_thread.start(&error_event_queue);
  controller_event_thread.start(&contoller_event_queue_thread);
  data_event_thread.start(&datalink_thread);
  monitor.start(&monitoring_thread);
  
  


  while(1){Thread::wait(1000000);};
  return 0;
}
