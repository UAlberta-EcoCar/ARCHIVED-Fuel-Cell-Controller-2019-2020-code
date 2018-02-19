#include <mbed.h>
#include <mbed_events.h>

// Defs
#include "Def/object_def.h"

#include "monitoring.h"
#include "main.h"

EventQueue cont_queue(32*EVENTS_EVENT_SIZE);

void start_state();
void start_purge();
void start_res_start();
void start_res_end();
void charge_state();
void charge_start();
void charge_stop();
void run_state();
void shut_state();
void alarm_state();
void update_leds();
void purge();
void test();

void contoller_event_queue_thread(){
  #ifdef ENABLE_TESTMODE
  cont_queue.call(test);
  #endif
  #ifndef ENABLE_TESTMODE
  cont_queue.call(shut_state);
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
}

// Start state setup
void start_state(){
  controller_flags.clear((CLEAR_EVENT_FLAG|CLEAR_SIGNAL_FLAG|CLEAR_FAN_FLAG));
  controller_flags.set((START_EVENT_FLAG|FAN_MAX_FLAG));
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
  controller_flags.set((FINISHED_EXCUTION_FLAG|SIGNAL_STARTSETUPCOMPLETE));
}

void start_purge(){
  controller_flags.clear(FINISHED_EXCUTION_FLAG);
  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_MAX_FLAG);
  // Startup purge
  supply_v.write(true);
  Thread::wait(1000);
  purge_v.write(true);
  Thread::wait(200);
  purge_v.write(false);

  controller_flags.set((SIGNAL_STARTPURGECOMPLETE|FINISHED_EXCUTION_FLAG));
}

void start_res_start(){
  controller_flags.clear(FINISHED_EXCUTION_FLAG);
  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_MAX_FLAG);
  // Start resistors
  cap_r.write(false);
  charge_r.write(false);
  Thread::wait(100);
  start_r.write(true);

  controller_flags.set((SIGNAL_STARTRESSTARTED|FINISHED_EXCUTION_FLAG));
}

void start_res_end(){
  controller_flags.clear(FINISHED_EXCUTION_FLAG);
  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_MAX_FLAG);

  start_r.write(false);
  Thread::wait(100);

  controller_flags.set((SIGNAL_STARTRESCOMPLETE|FINISHED_EXCUTION_FLAG));
}

void charge_state(){
  // Sinal charge state
  controller_flags.clear((CLEAR_EVENT_FLAG|CLEAR_SIGNAL_FLAG|CLEAR_FAN_FLAG));
  controller_flags.set((CHARGE_EVENT_FLAG|FAN_MIN_FLAG));
  fc.set_fc_status(CHARGE_STATE);

  update_leds();
  // For safety
  start_r.write(false);
  cap_r.write(false);

  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set((FINISHED_EXCUTION_FLAG|FAN_MIN_FLAG|SIGNAL_CHARGESETUPCOMPLETE));
}

void charge_start(){
  controller_flags.clear(FINISHED_EXCUTION_FLAG);
  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_MIN_FLAG);
  // Charging
  charge_r.write(true);

  controller_flags.set((FINISHED_EXCUTION_FLAG|SIGNAL_CHARGESTARTED));
}

void charge_stop(){
  controller_flags.clear(FINISHED_EXCUTION_FLAG);
  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_MIN_FLAG);

  charge_r.write(false);

  controller_flags.set((FINISHED_EXCUTION_FLAG|SIGNAL_CHARGECOMPLETED));

}

void run_state(){
  controller_flags.clear((CLEAR_EVENT_FLAG|CLEAR_SIGNAL_FLAG|CLEAR_FAN_FLAG));
  controller_flags.set((RUN_EVENT_FLAG|FAN_PID_FLAG));
  fc.set_fc_status(RUN_STATE);

  update_leds();
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(true);
  charge_r.write(false);
  cap_r.write(true);

  controller_flags.set(FINISHED_EXCUTION_FLAG);
}

void shut_state(){
  controller_flags.clear((CLEAR_EVENT_FLAG|CLEAR_SIGNAL_FLAG|CLEAR_FAN_FLAG));
  controller_flags.set((SHUT_EVENT_FLAG|FAN_SHUTDOWN_FLAG));
  fc.set_fc_status(SHUTDOWN_STATE);

  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  controller_flags.set(FINISHED_EXCUTION_FLAG);
}

void alarm_state(){
  controller_flags.clear((CLEAR_EVENT_FLAG|CLEAR_SIGNAL_FLAG|CLEAR_FAN_FLAG));
  controller_flags.set((ALARM_EVENT_FLAG|FAN_SHUTDOWN_FLAG));
  fc.set_fc_status(ALARM_STATE);

  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  controller_flags.set(FINISHED_EXCUTION_FLAG);
}

void purge(){
  controller_flags.clear(FINISHED_EXCUTION_FLAG);
  controller_flags.set(PURGE_EVENT_FLAG);

  fc.increment_purge();
  purge_v.write(true);
  Thread::wait(200);
  purge_v.write(false);

  controller_flags.clear(PURGE_EVENT_FLAG);
  controller_flags.set(FINISHED_EXCUTION_FLAG);
}
#ifdef ENABLE_TESTMODE
void test(){
  supply_v.write(true);
  purge_v.write(true);
  start_r.write(true);
  motor_r.write(true);
  charge_r.write(true);
  cap_r.write(true);
  alarm_led.write(true);
  shut_led.write(true);
  start_led.write(true);
  run_led.write(true);
  controller_flags.set(FAN_MIN_FLAG);
}
#endif