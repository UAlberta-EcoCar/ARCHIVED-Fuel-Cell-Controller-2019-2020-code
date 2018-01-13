#include <mbed.h>
#include <mbed_events.h>

#include "controller_event_queue.h"
#include "datalink.h"

EventQueue err_queue(32*EVENTS_EVENT_SIZE);
Event<void()> alarm_event(&err_queue, alarm_state);
Event<void()> error_log_event(&err_queue, error_logging);

void error_cleanup(){
  error_log_event.post();
  alarm_event.post();
}

void error_event_queue(){
  err_queue.dispatch();
  while(1){Thread::wait(100000.0);}
}
