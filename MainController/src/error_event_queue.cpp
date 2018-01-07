#include <mbed.h>
#include <mbed_events.h>

#include "controller_states.h"

EventQueue err_queue(32*EVENTS_EVENT_SIZE);
Ticker push_error_checks;

void error_checking(){

}

void error_cleanup(){
  err_queue.call(shutdown_state);
}

void error_event_queue(){

  push_error_checks.attach(error_checking, 0.5);
  err_queue.dispatch();
  while(1){Thread::wait(100000.0);}
}
