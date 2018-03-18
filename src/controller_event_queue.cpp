#include <mbed.h>
#include <mbed_events.h>

// Defs
#include "Def/object_def.h"

#include "monitoring.h"
#include "main.h"

EventQueue cont_queue(32*EVENTS_EVENT_SIZE);
Event<void()> state_event(&cont_queue, state_monitoring);
void update_leds();

// START
void start_state();
void start_purge();
void fc_charge_exit();

// RUN
void charge_state();
void cap_charge_entry();
void cap_charge_exit();

// RUN
void run_state();

// PURGE
void purge();

// SHUTDOWN AND ALARM
void shut_state();
void alarm_state();

// TEST STATE
// ESSENTIALLY ALLOWS A MINI PROGRAM TO BE RUN WITHIN THE TEST FUNCTION.
// MEANT TO BE MODIFIED AND USED HOWEVER NEEDED.
void test();

void contoller_event_queue_thread(){
  #ifdef ENABLE_TESTSTATE
  cont_queue.call(test);
  #endif
  #ifndef ENABLE_TESTSTATE
  cont_queue.call(shut_state);
  #endif

  cont_queue.dispatch_forever();
}

void update_leds(){
  alarm_led.write(false);
  shut_led.write(false); // Red
  start_led.write(false); // Blue
  run_led.write(false); // Green
  fc.lock();
  int state = fc.get_fc_status();
  fc.unlock();

  if (state == ALARM_STATE){
    alarm_led.write(true);
  }
  else if (state == SHUTDOWN_STATE){
    shut_led.write(true);
  }
  else if (state == START_STATE){
    start_led.write(true);
  }
  else if (state == CHARGE_STATE){
    start_led.write(true);
    run_led.write(true);
  }
  else if (state == RUN_STATE){
    run_led.write(true);
  }
  else if (state == PURGE_STATE){
    shut_led.write(true);
    run_led.write(true);
  }
}

// Start state setup
void start_state(){
  // Header
  controller_flags.clear();
  fc.set_fc_status(START_STATE);

  // Transition
  update_leds();
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  
  // Trailer
  controller_flags.set((SIGNAL_STARTSETUPCOMPLETE|FAN_MAX_FLAG));
  state_event.post();
}

void start_purge(){
  // Header
  controller_flags.clear();
  // Enable low pressure error

  // Transition
  purge_v.write(false);
  start_r.write(true);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  supply_v.write(true);
  Thread::wait(1000);
  purge_v.write(true);
  Thread::wait(200);
  purge_v.write(false);

  // Trailer
  controller_flags.set((SIGNAL_STARTPURGECOMPLETE|FAN_MAX_FLAG));
  state_event.post();
}

void fc_charge_exit(){
  // Header
  controller_flags.clear();

  // Transition
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  // Trailer
  controller_flags.set((SIGNAL_STATETRANSITION|FAN_MAX_FLAG));
  state_event.post();
}

void charge_state(){
  // Header
  controller_flags.clear();
  fc.set_fc_status(CHARGE_STATE);

  // Transition
  update_leds();
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  // Trailer
  controller_flags.set((FAN_MIN_FLAG|SIGNAL_CHARGESETUPCOMPLETE));
  state_event.post();
}

void cap_charge_entry(){
  // Header
  controller_flags.clear();

  // Transistion
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  cap_r.write(false);
  charge_r.write(true);

  // Trailer
  controller_flags.set((FAN_MIN_FLAG|SIGNAL_CHARGESTARTED));
  state_event.post();
}

void cap_charge_exit(){
  // Header
  controller_flags.clear();

  // Transistion
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  // Trailer
  controller_flags.set((SIGNAL_STATETRANSITION|FAN_MIN_FLAG));
  state_event.post();
}

void run_state(){
  // Header
  controller_flags.clear();
  fc.set_fc_status(RUN_STATE);

  // Transition
  update_leds();
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(true);
  charge_r.write(false);
  cap_r.write(true);

  // Trailer
  controller_flags.set(FAN_PID_FLAG);
  state_event.post();
}

void purge(){
  // Header
  controller_flags.clear();
  fc.set_fc_status(PURGE_STATE);
  fc.increment_purge();

  // Transition
  update_leds();
  supply_v.write(true);
  purge_v.write(true);
  start_r.write(false);
  motor_r.write(true);
  charge_r.write(false);
  cap_r.write(true);
  Thread::wait(200);
  purge_v.write(false);

  // Trailer
  controller_flags.set((SIGNAL_STATETRANSITION|FAN_PID_FLAG));
  state_event.post();
}

void shut_state(){
  controller_flags.clear();
  fc.set_fc_status(SHUTDOWN_STATE);

  // Transition 
  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  // Trailer
  controller_flags.set(FAN_SHUTDOWN_FLAG);
  state_event.post();
}

void alarm_state(){
  // Header
  controller_flags.clear();
  fc.set_fc_status(ALARM_STATE);

  // Transition
  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  // Trailer
  controller_flags.set(FAN_SHUTDOWN_FLAG);
  state_event.post();
}

#ifdef ENABLE_TESTSTATE
void test(){
  supply_v.write(true);
  purge_v.write(false);
  controller_flags.set(FAN_MIN_FLAG);  
}
#endif