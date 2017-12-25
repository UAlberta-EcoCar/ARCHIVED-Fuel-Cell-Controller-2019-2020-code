#include <mbed.h>
#include <mbed_events.h>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"
#include "Classes/LinearScalable.h"

// Defs
#include "Def/constants.h"
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"
#include "Def/semaphore_def.h"

// Thread src
#include "controller_event_queue.h"
#include "controller_states.h"
#include "monitoring.h"
#include "error_event_queue.h"
#include "main.h"

#include "fc_status.h"
/*
Initilaize Objects
  -These are used by the Threads
    -want the I/O to be global, local objects are defined locally
*/

// Interrupt Objects
InterruptIn h2(H2_OK);
InterruptIn err(ERROR_ISR);

// Scale objects
LinearScalable cap_scale(1.0, 2.0);

// AnalogIn_Ext Objects
Analog_Sensor<LinearScalable> capvolt(CAPVOLT, cap_scale, "capvolt");
Analog_Sensor<LinearScalable> fccurr(FCCURR, cap_scale, "fccurr");
Analog_Sensor<LinearScalable> fcvolt(FCVOLT, cap_scale, "fcvolt");
Analog_Sensor<LinearScalable> capcurr(CAPCURR, cap_scale, "capcurr");
Analog_Sensor<LinearScalable> motorvolt(MOTORVOLT, cap_scale, "motorvolt");
Analog_Sensor<LinearScalable> motorcurr(MOTORCURR, cap_scale, "motorcurr");
Analog_Sensor<LinearScalable> press1(PRESS1, cap_scale, "press1");
Analog_Sensor<LinearScalable> press2(PRESS2, cap_scale, "press2");
Analog_Sensor<LinearScalable> press3(PRESS3, cap_scale, "press3");
Analog_Sensor<LinearScalable> press4(PRESS4, cap_scale, "press4");
Analog_Sensor<LinearScalable> fctemp1(FCTEMP1, cap_scale, "fctemp1");
Analog_Sensor<LinearScalable> fctemp2(FCTEMP2, cap_scale, "fctemp2");
Analog_Sensor<LinearScalable> temp1(TEMP1, cap_scale, "temp1");
Analog_Sensor<LinearScalable> temp2(TEMP2, cap_scale, "temp2");
Analog_Sensor<LinearScalable> temp3(TEMP3, cap_scale, "temp3");
Analog_Sensor<LinearScalable> temp4(TEMP4, cap_scale, "temp4");
Analog_Sensor<LinearScalable> temp5(TEMP5, cap_scale, "temp5");

// DigitalOut_Ext objects
DigitalOut_Ext supply_v(SUPPLY_V);
DigitalOut_Ext purge_v(PURGE_V);
DigitalOut_Ext other1_v(VALVE3);
DigitalOut_Ext other2_v(VALVE4);
DigitalOut_Ext start_r(START_R);
DigitalOut_Ext motor_r(MOTOR_R);
DigitalOut_Ext charge_r(CHARGE_R);
DigitalOut_Ext cap_r(CAP_R);
DigitalOut_Ext fcc_r(FCC_R);
DigitalOut_Ext error_throw(ERROR_ISR_THROW);
DigitalOut_Ext alarm_led(ALARM_LED);
DigitalOut_Ext debug_led(DEBUG_LED);
DigitalOut_Ext shut_led(SHUT_LED);
DigitalOut_Ext run_led(RUN_LED);
DigitalOut_Ext start_led(START_LED);
DigitalOut_Ext ol_rst(OL_RST);
DigitalOut_Ext hum_rst(HUM_RST);

// Fan objects
Fan fan1(PWM_1,TACH_1);
Fan fan2(PWM_2,TACH_2);
Fan fan3(PWM_3,TACH_3);

// Integrator objects
Integrator fc_coulumbs;
Integrator fc_joules;
Integrator cap_coulumbs;
Integrator cap_joules;


// Initilaize threads
Thread controller_event_thread;
Thread error_event_thread;
Thread data_event_thread;
Thread FTDI_event_thread;
Thread monitor;

void error_isr(){
  controller_event_thread.terminate();
  set_fc_status(ALARM_STATE);
  error_cleanup();
}

int main() {




  // Attach Interrupts (Should be the first thing to do)
  //h2.fall(&error_isr);
  //err.rise(&error_isr);


  // Threads from lowest -> highest priority
  monitor.set_priority(osPriorityIdle); // Will be running 90% of the time, since other threads are quick
  data_event_thread.set_priority(osPriorityLow);
  controller_event_thread.set_priority(osPriorityBelowNormal);
  FTDI_event_thread.set_priority(osPriorityNormal);
  error_event_thread.set_priority(osPriorityHigh);

  // Start threads
  error_event_thread.start(&error_event_queue);
  controller_event_thread.start(&contoller_event_queue_thread);
  monitor.start(&monitoring_thread);


  while(1){Thread::wait(1000000);};
  return 0;
}
