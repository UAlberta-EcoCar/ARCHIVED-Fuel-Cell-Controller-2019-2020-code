#include <mbed.h>
#include <rtos.h>
#include <mbed_events.h>

#include "private_lib/digital_io.h"
#include "private_lib/pin_def.h"
#include "private_lib/fan_control.h"
#include "private_lib/fc_status.h"
#include "private_lib/constants.h"

#include "controller_states.h"

EventQueue cont_queue(32*EVENTS_EVENT_SIZE);

void push_function(functiontype func){
  cont_queue.call(func);
}
void test(){
  cont_queue.call(test_state);
}

void startup(){
  set_fc_status(START_STATE);
  cont_queue.call(fan_spool_up);

}
void run(){
  set_fc_status(RUN_STATE);
  cont_queue.call(run_setup);
}

void shutdown(){
  set_fc_status(SHUTDOWN_STATE);
  cont_queue.call(shutdown_state);
}

void alarm(){
  set_fc_status(ALARM_STATE);
  cont_queue.call(shutdown_state);
}

int contoller_event_queue_thread(){
  cont_queue.call(shutdown);
  cont_queue.dispatch();


  while(1){Thread::wait(10);}
  return 1;
}
