#include <mbed.h>
#include <mbed_events.h>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"

// Defs
#include "Def/constants.h"
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"

#include "fc_status.h"
#include "controller_states.h"

EventQueue cont_queue(32*EVENTS_EVENT_SIZE);

void push_function(functiontype func){
  cont_queue.call(func);
}
void test(){
  cont_queue.call(test_state);
}

void run(){
  set_fc_status(RUN_STATE);
  alarm_led.write_safe(false);
  shut_led.write_safe(false);
  start_led.write_safe(false);
  run_led.write_safe(true);
  cont_queue.call(run_setup);
}

void startup(){
  set_fc_status(START_STATE);
  alarm_led.write_safe(false);
  shut_led.write_safe(false);
  start_led.write_safe(true);
  run_led.write_safe(false);

  cont_queue.call(fan_spool_up);
  cont_queue.call(start_purge);
  cont_queue.call(start_charge);
  cont_queue.call(start_end);
  cont_queue.call(run);
}

void shutdown(){
  set_fc_status(SHUTDOWN_STATE);
  alarm_led.write_safe(false);
  shut_led.write_safe(true);
  start_led.write_safe(false);
  run_led.write_safe(false);
  cont_queue.call(shutdown_state);
}

void alarm(){
  set_fc_status(ALARM_STATE);
  alarm_led.write_safe(true);
  shut_led.write_safe(false);
  start_led.write_safe(false);
  run_led.write_safe(false);
  cont_queue.call(shutdown_state);
}

int contoller_event_queue_thread(){
  cont_queue.call(run);
  cont_queue.dispatch();


  while(1){Thread::wait(10000);}
  return 1;
}
