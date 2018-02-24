#include <mbed.h>
#include <mbed_events.h>

// Classes
#include "Classes/LinearScalable.h"
#include "Classes/ExpScalable.h"
#include "Classes/PolyScalable.h"

// Defs
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"

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

#ifdef ENABLE_RTC
RealTimeClock rtc("clock", &master);
#endif

#ifdef ENABLE_SHT31
SHT31 sht31("SHT31", &master);
#endif

// Scale objects
LinearScalable<float> v_s(45.768, 0.2715);
LinearScalable<float> c_s(48.329, -15.464);
LinearScalable<float> press_s(49.65, -24.468);
PolyScalable<float> fctemp_s(140.0978, -899.05152, 3595.492, -7539.7506, 7471.2785, -2818.1076);

// AnalogIn_Ext Objects
Analog_Sensor<LinearScalable<float> > capvolt(CAPVOLT, v_s, "capvolt");
Analog_Sensor<LinearScalable<float> > fccurr(FCCURR, c_s, "fccurr");
Analog_Sensor<LinearScalable<float> > fcvolt(FCVOLT, v_s, "fcvolt");
Analog_Sensor<LinearScalable<float> > capcurr(CAPCURR, c_s, "capcurr");
Analog_Sensor<LinearScalable<float> > motorvolt(MOTORVOLT, v_s, "motorvolt"); 
Analog_Sensor<LinearScalable<float> > motorcurr(MOTORCURR, c_s, "motorcurr");
Analog_Sensor<LinearScalable<float> > press1(PRESS1, press_s, "press1");

#ifdef ENABLE_PRESS2
Analog_Sensor<LinearScalable<float> > press2(PRESS2, cap_scale, "press2");
#endif
#ifdef ENABLE_PRESS3
Analog_Sensor<LinearScalable<float> > press3(PRESS3, cap_scale, "press3");
#endif
#ifdef ENABLE_PRESS4
Analog_Sensor<LinearScalable<float> > press4(PRESS4, cap_scale, "press4");
#endif

Analog_Sensor<PolyScalable<float> > fctemp1(FCTEMP1, fctemp_s, "fctemp1");

#ifdef ALICE_CONFIGURATION
Analog_Sensor<PolyScalable<float> > fctemp2(FCTEMP2, fctemp_s, "fctemp2");
#endif

#ifdef ENABLE_TEMP1
Analog_Sensor<LinearScalable<float> > temp1(TEMP1, cap_scale, "temp1");
#endif
#ifdef ENABLE_TEMP2
Analog_Sensor<LinearScalable<float> > temp2(TEMP2, cap_scale, "temp2");
#endif
#ifdef ENABLE_TEMP3
Analog_Sensor<LinearScalable<float> > temp3(TEMP3, cap_scale, "temp3");
#endif
#ifdef ENABLE_TEMP4
Analog_Sensor<LinearScalable<float> > temp4(TEMP4, cap_scale, "temp4");
#endif
#ifdef ENABLE_TEMP5
Analog_Sensor<LinearScalable<float> > temp5(TEMP5, cap_scale, "temp5");
#endif

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

#ifdef ALICE_CONFIGURATION
Fan fan2("fan2",PWM_2,TACH_2);
Fan fan3("fan3",PWM_3,TACH_3);
#endif

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
  
  // Threads from lowest -> highest priority
  monitor.set_priority(osPriorityAboveNormal7);
  controller_event_thread.set_priority(osPriorityAboveNormal);
    data_event_thread.set_priority(osPriorityHigh);
  error_event_thread.set_priority(osPriorityRealtime1);
  error_event_low_thread.set_priority(osPriorityRealtime7);

  //Start threads
  #ifdef ENABLE_ERRORS
  error_event_thread.start(&error_event_queue);
  error_event_low_thread.start(&error_event_queue_low);
  #endif

  controller_event_thread.start(&contoller_event_queue_thread);
  monitor.start(&monitoring_thread);

  #ifdef ENABLE_DATALOGGING
  data_event_thread.start(&datalink_thread);
  #endif
  
  return 0;
}
