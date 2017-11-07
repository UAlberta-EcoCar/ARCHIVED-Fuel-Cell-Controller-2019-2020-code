#include <mbed.h>
#include <rtos.h>
#include <mbed_events.h>

#include "private_lib/digital_io.h"
#include "private_lib/pin_def.h"
#include "private_lib/signal_def.h"
#include "private_lib/fan_control.h"
#include "private_lib/fc_status.h"
#include "private_lib/constants.h"

#include "controller_states.h"

EventQueue queue(32*EVENTS_EVENT_SIZE);

void push_function(functiontype func){
  queue.call(func);
}

void standby(){
  set_fc_status(STANDBY_STATE);
  queue.call(standby_state);

}
void startup(){
  set_fc_status(START_STATE);
  queue.call(fan_spool_up);
  queue.call(start_purge);
  queue.call(start_end);
  queue.call(start_charge);
}
void run(){
  set_fc_status(RUN_STATE);
  queue.call(run_setup);
}

void shutdown(){
  set_fc_status(SHUTDOWN_STATE);
  queue.call(shutdown_state);
}

void alarm(){
  set_fc_status(ALARM_STATE);
  queue.call(shutdown_state);
}

int contoller_event_queue_thread(){
  queue.call(standby);
  queue.dispatch();


  while(1){Thread::wait(10);}
  return 1;
}
