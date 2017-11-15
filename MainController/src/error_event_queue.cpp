#include <mbed.h>
#include <mbed_events.h>

#include "controller_states.h"

EventQueue err_queue(32*EVENTS_EVENT_SIZE);

void error_cleanup(){
  err_queue.call(shutdown_state);
}

void error_event_queue(){
  err_queue.dispatch();

  while(1){Thread::wait(10);}
}
