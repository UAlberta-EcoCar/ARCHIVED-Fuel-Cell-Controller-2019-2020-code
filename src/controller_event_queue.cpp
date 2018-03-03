#include <mbed.h>
#include <mbed_events.h>

// Defs
#include "Def/object_def.h"

#include "monitoring.h"
#include "main.h"

EventQueue cont_queue(32*EVENTS_EVENT_SIZE);

Event<void()> state_event(&cont_queue, state_monitoring);

void start_state();
void start_purge();
void fc_charge_entry();
void fc_charge_exit();
void charge_state();
void cap_charge_entry();
void cap_charge_exit();
void run_state();
void purge();
void shut_state();
void alarm_state();
void update_leds();
void test();

void contoller_event_queue_thread(){
  #ifdef ENABLE_TESTMODE
  cont_queue.call(test);
  #endif
  #ifndef ENABLE_TESTMODE
  cont_queue.call(start_state);
  #endif

  cont_queue.dispatch_forever();
}

void update_leds(){
  alarm_led.write(false);
  shut_led.write(false);
  start_led.write(false);
  run_led.write(false);
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
  controller_flags.clear();
  fc.set_fc_status(START_STATE);

  // Move this to a monitoring task
  update_leds();

  // Close all valves, Open all relays
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  
  // Signal setup complete
  controller_flags.set((SIGNAL_STARTSETUPCOMPLETE|FAN_MAX_FLAG));
  state_event.post();
}

void start_purge(){
  // Header
  controller_flags.clear();

  // Event
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  supply_v.write(true);
  Thread::wait(1000);
  purge_v.write(true);
  Thread::wait(200);
  purge_v.write(false);

  // Footer
  controller_flags.set((SIGNAL_STARTPURGECOMPLETE|FAN_MAX_FLAG));
  state_event.post();
}

void fc_charge_entry(){
  // Header
  controller_flags.clear();

  // Start resistors
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  Thread::wait(100);
  start_r.write(true);

  // Footer
  controller_flags.set((SIGNAL_FCCHARGESTARTED|FAN_MAX_FLAG));
  state_event.post();
}

void fc_charge_exit(){
  controller_flags.clear();

  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  Thread::wait(100);

  controller_flags.set((SIGNAL_STATETRANSITION|FAN_MAX_FLAG));
  //cont_queue.call(charge_state);
}

void charge_state(){
  controller_flags.clear();
  fc.set_fc_status(CHARGE_STATE);

  update_leds();
  // For safety
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  controller_flags.set((FAN_MIN_FLAG|SIGNAL_CHARGESETUPCOMPLETE));
  state_event.post();
}

void cap_charge_entry(){
  controller_flags.clear();
  // Charging
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  cap_r.write(false);
  charge_r.write(true);


  controller_flags.set((FAN_MIN_FLAG|SIGNAL_CHARGESTARTED));
  state_event.post();
}

void cap_charge_exit(){
  controller_flags.clear();

  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  controller_flags.set((SIGNAL_STATETRANSITION|FAN_MIN_FLAG));
  //cont_queue.call(run_state);
}

void run_state(){
  controller_flags.clear();
  fc.set_fc_status(RUN_STATE);
  update_leds();
  
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(true);
  charge_r.write(false);
  cap_r.write(true);

  controller_flags.set(FAN_PID_FLAG);
  state_event.post();
}

void purge(){
  // Header
  controller_flags.clear();
  fc.set_fc_status(PURGE_STATE);
  fc.increment_purge();
  update_leds();

  // Event
  supply_v.write(true);
  purge_v.write(true);
  start_r.write(false);
  motor_r.write(true);
  charge_r.write(false);
  cap_r.write(true);
  Thread::wait(200);
  purge_v.write(false);

  // Footer
  controller_flags.set((SIGNAL_STATETRANSITION|FAN_PID_FLAG));
  //cont_queue.call(run_state);
}

void shut_state(){
  controller_flags.clear();
  fc.set_fc_status(SHUTDOWN_STATE);

  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  controller_flags.set(FAN_SHUTDOWN_FLAG);
}

void alarm_state(){
  controller_flags.clear();
  fc.set_fc_status(ALARM_STATE);

  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  controller_flags.set(FAN_SHUTDOWN_FLAG);
}

#ifdef ENABLE_TESTMODE
void test(){
  supply_v.write(true);
  Thread::wait(5000);
  purge_v.write(true);
  Thread::wait(1000);
  start_r.write(true);
  Thread::wait(1000);
  motor_r.write(true);
  Thread::wait(1000);
  charge_r.write(true);
  Thread::wait(1000);
  cap_r.write(true);
  Thread::wait(1000);
  alarm_led.write(true);
  shut_led.write(true);
  start_led.write(true);
  run_led.write(true);
  controller_flags.set(FAN_MIN_FLAG);  
}
#endif