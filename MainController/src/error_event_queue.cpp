#include <mbed.h>
#include <mbed_events.h>

#include "controller_event_queue.h"
#include "datalink.h"

EventQueue err_queue(32*EVENTS_EVENT_SIZE);

void error_cleanup(){
  err_queue.call(shutdown_state);
  err_queue.call(error_logging);
}

void error_event_queue(){
  err_queue.dispatch();
  while(1){Thread::wait(100000.0);}
}
