#include <mbed.h>
#include <mbed_events.h>

#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"

#include "controller_event_queue.h"
#include "datalink.h"
#include "main.h"

// Sofie
#ifndef ALICE_CONFIGURATION
#define MAX_FC_CURR 30
#define MAX_FC_VOLT 34
#define MIN_FC_VOLT 10
#define MAX_CAP_CURR 30
#define MAX_CAP_VOLT 32.0
#define MAX_PRESS 11.0
#define MIN_PRESS 3.0
#endif

// Alice
#ifdef ALICE_CONFIGURATION
#define MAX_FC_CURR 70
#define MAX_FC_VOLT 50.6
#define MAX_CAP_CURR 70
#define MAX_CAP_VOLT 51.8
#define MAX_PRESS 11.0
#define MIN_PRESS 3.0
#endif



EventQueue err_queue(32*EVENTS_EVENT_SIZE);
EventQueue err_queue_low(32*EVENTS_EVENT_SIZE);

Timer low_pressure;

void error_handler(int error_code);
void overvoltage_check();
void undervoltage_check();
void h2estop_check();
void overcurrent_check();
void pressure_check();
void temp_check();
void post_checks();

// Higher Pri events
Event<void(int)> error_handler_event(&err_queue, error_handler);
Event<void()> alarm_event(&err_queue, alarm_state);

// Lower Pri events
Event<void()> overvoltage_check_event(&err_queue_low, overvoltage_check);
Event<void()> undervoltage_check_event(&err_queue_low, undervoltage_check);
Event<void()> h2estop_check_event(&err_queue_low, h2estop_check);
Event<void()> overcurrent_check_event(&err_queue_low, overcurrent_check);
Event<void()> pressure_check_event(&err_queue_low, pressure_check);
Event<void()> temp_check_event(&err_queue_low, temp_check);
Event<void()> post_checks_event(&err_queue_low, post_checks);

// External Queue Events
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

  #ifdef ENABLE_OVERVOLTAGE
  if (fcvolt.read() >= MAX_FC_VOLT || capvolt.read() >= MAX_CAP_VOLT){
    error_handler_event.post(OVERVOLTAGE);
    return;
  }
  #endif
}

void undervoltage_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  #ifdef ENABLE_UNDERVOLTAGE
  if (fcvolt.read() <= MIN_FC_VOLT && fc.get_fc_status() == RUN_STATE){
    error_handler_event.post(UNDERVOLTAGE);
    return;
  }
  #endif
}

void h2estop_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  #ifdef ENABLE_H2ESTOP
  if (STOP == false){
    error_handler_event.post(H2ESTOP);
    return;
  }
  #endif
}

void overcurrent_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  #ifdef OVERCURR
  if (fccurr.read() >= 70 || capcurr.read() >= 70){
    error_handler_event.post(OVERCURR);
    return;
  }
  #endif
}

void pressure_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  #ifdef ENABLE_OVERPRESS
  if (press1.read() >= MAX_PRESS){
    error_handler_event.post(OVERPRESS);
    return;
  }
  #endif

  #ifdef ENABLE_UNDERPRESS
  if (press1.read() <= MIN_PRESS && fc.get_fc_status() != SHUTDOWN_STATE){
    low_pressure.start();
  }
  else{
    low_pressure.stop();
    low_pressure.reset();
  }

  if (low_pressure.read() > 2.0){
    error_handler_event.post(UNDERPRESS);
    return;
  }
  #endif
}

void temp_check(){
  if (fc.get_error_status() != NOERROR){
    return;
  }

  fccurr.lock();
  float curr = fccurr.read();
  fccurr.unlock();

  #ifdef  ENABLE_OVERTEMP
  #ifdef ALICE_CONFIGURATION
  if (fctemp1.read() >= fc.query_max_temp(curr) || fctemp2.read() >= fc.query_max_temp(curr)){
    error_handler_event.post(OVERTEMP);
    return;
  }
  #endif

  #ifndef ALICE_CONFIGURATION
  if (fctemp1.read() >= fc.query_max_temp(curr)){
    error_handler_event.post(OVERTEMP);
    return;
  }
  #endif
  #endif

  #ifdef  ENABLE_UNDERTEMP
  #ifdef ALICE_CONFIGURATION
  if (fctemp1.read() >= fc.query_min_temp(curr) || fctemp2.read() >= fc.query_min_temp(curr)){
    error_handler_event.post(UNDERTEMP);
    return;
  }
  #endif
  #ifndef ALICE_CONFIGURATION
  if (fctemp1.read() >= fc.query_min_temp(curr)){
    error_handler_event.post(UNDERTEMP);
    return;
  }
  #endif
  #endif
}

void post_checks(){
  if (fc.get_fc_status() != ALARM_STATE){
    overvoltage_check_event.post();
    undervoltage_check_event.post();
    h2estop_check_event.post();
    overcurrent_check_event.post();
    pressure_check_event.post();
    temp_check_event.post();
  }
}

void error_handler(int error_code){
  // Enter critical section
  __disable_irq();

  cont_queue.break_dispatch();
  err_queue_low.break_dispatch();
  controller_event_thread.terminate();
  fc.set_error_status(error_code);
  alarm_event.post();
  error_log_event.post();

  // Dettach Interrupts
  #ifdef ENABLE_H2STOP
  h2.fall(NULL);
  #endif
  #ifdef ENABLE_ESTOP1
  estop1.fall(NULL);
  #endif
  #ifdef ENABLE_ESTOP2
  estop2.fall(NULL);
  #endif

  __enable_irq();
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

void stop_isr(){
  error_handler_event.post(STOP_ALARM);
}

void error_event_queue(){
  err_queue.dispatch_forever();
}

void error_event_queue_low(){
  // Attach Interrupts
  #ifdef ENABLE_H2STOP
  h2.fall(&h2_isr); // Change to fall
  h2.mode(PullDown);
  #endif

  #ifdef ENABLE_ESTOP1
  estop1.rise(&estop1_isr); // Change to fall
  estop1.mode(PullUp);
  #endif

  #ifdef ENABLE_ESTOP2
  estop2.rise(&estop2_isr); // Change to fall
  estop2.mode(PullUp);
  #endif

  #ifdef ENABLE_STOP
  stop.fall(&stop_isr);
  stop.mode(PullUp);
  #endif

  post_checks_event.period(10);
  post_checks_event.post();
  err_queue_low.dispatch_forever();
}
