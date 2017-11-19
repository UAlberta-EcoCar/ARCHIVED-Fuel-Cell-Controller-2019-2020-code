#include <mbed.h>
#include <mbed_events.h>

#include "private_lib/AnalogIn_Ext.h"
#include "private_lib/constants.h"
#include "private_lib/pin_def.h"
#include "private_lib/fc_status.h"
#include "controller_event_queue.h"
#include "controller_states.h"
#include "monitoring.h"
#include "error_event_queue.h"
#include "main.h"

Thread controller_event_thread;
Thread error_event_thread;
Thread data_event_thread;
Thread FTDI_event_thread;
Thread monitor;

InterruptIn h2(H2_OK);
InterruptIn err(ERROR_ISR);

void error_isr(){
  controller_event_thread.terminate();
  set_fc_status(ALARM_STATE);
  error_cleanup();
}

int main() {
  // Create Interrupts (Should be the first thing to do)
  h2.fall(&error_isr);
  err.rise(&error_isr);

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
