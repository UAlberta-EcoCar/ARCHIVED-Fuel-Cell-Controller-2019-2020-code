#include <mbed.h>
#include <mbed_events.h>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"
#include "Classes/LinearScalable.h"
#include "Classes/ExpScalable.h"
#include "Classes/PolyScalable.h"
#include "Classes/SerialPrinter.h"
#include "Classes/HumiditySensor.h"
#include "Classes/RTC.h"
#include "Classes/FuelCell.h"
#include "Classes/FanControl.h"

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
//I2C Objects
I2C master(I2C_SDA, I2C_SCL);
HumiditySensor humidity("Humidity", &master);

// Scale objects
LinearScalable<float> v_s(45.768, 0.2715);
LinearScalable<float> c_s(48.329, -15.464);
LinearScalable<float> press_s(49.65, -24.468);



// AnalogIn_Ext Objects
Analog_Sensor<LinearScalable<float> > capvolt(CAPVOLT, v_s, "capvolt");
Analog_Sensor<LinearScalable<float> > fccurr(FCCURR, c_s, "fccurr");
Analog_Sensor<LinearScalable<float> > fcvolt(FCVOLT, v_s, "fcvolt");
Analog_Sensor<LinearScalable<float> > capcurr(CAPCURR, c_s, "capcurr");
// Assuming current and voltage sensing is the same as fc for motor, might be a good assumption for cap as well
Analog_Sensor<LinearScalable<float> > motorvolt(MOTORVOLT, v_s, "motorvolt"); 
Analog_Sensor<LinearScalable<float> > motorcurr(MOTORCURR, c_s, "motorcurr");
Analog_Sensor<LinearScalable<float> > press1(PRESS1, press_s, "press1");
//Analog_Sensor<LinearScalable<float> > press2(PRESS2, cap_scale, "press2");
//Analog_Sensor<LinearScalable<float> > press3(PRESS3, cap_scale, "press3");
//Analog_Sensor<LinearScalable<float> > press4(PRESS4, cap_scale, "press4");
Analog_Sensor<LinearScalable<float> > fctemp1(FCTEMP1, c_s, "fctemp1");
Analog_Sensor<LinearScalable<float> > fctemp2(FCTEMP2, c_s, "fctemp2");
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
vector<Fan*> fan_vec;
vector<Analog_Sensor<LinearScalable<float> >* > temp_vec;

//Iterators
vector<Sensor*>::iterator sensor_iter;
vector<Integrator*>::iterator int_iter;
vector<DigitalOut_Ext*>::iterator dig_out_iter;
vector<Fan*>::iterator fan_iter;

FanControl<LinearScalable<float>, LinearScalable<float> > fan_cont(
    &fan_vec, 
    &temp_vec,
    &fccurr
);

// Initilaize threads
Thread controller_event_thread;
Thread error_event_thread;
Thread error_event_low_thread;
Thread data_event_thread;
Thread FTDI_event_thread;
Thread monitor;

int main() {
  //Populate vectors
  sensor_vec.push_back(&fccurr);
  sensor_vec.push_back(&capvolt);
  sensor_vec.push_back(&fcvolt);
  sensor_vec.push_back(&capcurr);
  sensor_vec.push_back(&motorvolt);
  sensor_vec.push_back(&motorcurr);
  sensor_vec.push_back(&press1);
  //sensor_vec.push_back(&press2);
  //sensor_vec.push_back(&press3);
  //sensor_vec.push_back(&press4);
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

  fan_vec.push_back(&fan1);
  fan_vec.push_back(&fan2);
  fan_vec.push_back(&fan3);

  fc_coulumbs.sensor_add(&fccurr);
  fc_joules.sensor_add(&fccurr);
  fc_joules.sensor_add(&fcvolt);
  cap_coulumbs.sensor_add(&capcurr);
  cap_joules.sensor_add(&capcurr);
  cap_joules.sensor_add(&capvolt);

  // Threads from lowest -> highest priority
  data_event_thread.set_priority(osPriorityLow);
  monitor.set_priority(osPriorityAboveNormal);
  controller_event_thread.set_priority(osPriorityHigh);
  error_event_thread.set_priority(osPriorityRealtime1);
  error_event_low_thread.set_priority(osPriorityRealtime7);

  //Start threads
  error_event_thread.start(&error_event_queue);
  error_event_low_thread.start(&error_event_queue_low);
  controller_event_thread.start(&contoller_event_queue_thread);
  monitor.start(&monitoring_thread);
  data_event_thread.start(&datalink_thread);
  return 0;
}
