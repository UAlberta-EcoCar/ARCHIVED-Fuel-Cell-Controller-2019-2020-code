#include <mbed.h>
#include <mbed_events.h>
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"
#include "controller_event_queue.h"
#include "datalink.h"

EventQueue err_queue(32*EVENTS_EVENT_SIZE);
EventQueue err_queue_low(32*EVENTS_EVENT_SIZE);

void error_handler(int error_code);
void overvoltage_check();
void overcurrent_check();
void pressure_check();
void temp_check();
void post_checks();

// Higher Pri events
Event<void(int)> error_handler_event(&err_queue, error_handler);

// Lower Pri events
Event<void()> overvoltage_check_event(&err_queue_low, overvoltage_check);
Event<void()> overcurrent_check_event(&err_queue_low, overcurrent_check);
Event<void()> pressure_check_event(&err_queue_low, pressure_check);
Event<void()> temp_check_event(&err_queue_low, temp_check);
Event<void()> post_checks_event(&err_queue_low, post_checks);

// External Queue Events
Event<void()> alarm_event(&err_queue, alarm_state);
Event<void()> error_log_event(&err_queue, error_logging);

// Interrupt Objects
InterruptIn h2(H2_OK);
InterruptIn stop(STOP);
InterruptIn estop1(ESTOP1);
InterruptIn estop2(ESTOP2);


void overvoltage_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  if (fcvolt.read() >= 30.8 || capvolt.read() >= 32.0){
    error_handler_event.post(OVERVOLTAGE);
  }
}

void overcurrent_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  if (fccurr.read() >= 70 || capcurr.read() >= 70){
    error_handler_event.post(OVERCURR);
  }
}

void pressure_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  if (press1.read() >= 8.0){
    error_handler_event.post(OVERPRESS);
  }

  if (press1.read() <= 5.0){
    error_handler_event.post(UNDERPRESS);
  }
}

void temp_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  fccurr.lock();
  float curr = fccurr.read();
  fccurr.unlock();

  if (fctemp1.read() >= fc.query_max_temp(curr) || fctemp2.read() >= fc.query_max_temp(curr)){
    error_handler_event.post(OVERTEMP);
  }

  if (fctemp1.read() >= fc.query_min_temp(curr) || fctemp2.read() >= fc.query_min_temp(curr)){
    error_handler_event.post(UNDERTEMP);
  }
}

void post_checks(){
  overvoltage_check_event.post();
  overcurrent_check_event.post();
  pressure_check_event.post();
  temp_check_event.post();
}

void error_handler(int error_code){
  cont_queue.break_dispatch();
  controller_event_thread.terminate();
  fc.set_error_status(error_code);
  alarm_event.post();
  error_log_event.post();
}

void h2_isr(){
  error_handler_event.post(H2_ALARM);
}

void estop1_isr(){
  error_handler_event.post(ESTOP1_ALARM);
}

void estop2_isr(){
  error_handler_event.post(ESTOP2_ALARM);
}

void error_event_queue(){
  err_queue.dispatch_forever();
}

void error_event_queue_low(){
  // Attach Interrupts
  h2.rise(&h2_isr);
  estop1.rise(&estop1_isr);
  estop2.rise(&estop2_isr);
  estop1.mode(PullDown);
  h2.mode(PullDown);
  estop2.mode(PullDown);

  post_checks_event.period(10);
  post_checks_event.post();
  err_queue_low.dispatch_forever();
}
