#include <mbed.h>
#include <mbed_events.h>

#include "Classes/DigitalOut_Ext.h"
#include "Classes/FuelCell.h"

// Defs
#include "Def/constants.h"
#include "Def/object_def.h"

#include "monitoring.h"

EventQueue cont_queue(32*EVENTS_EVENT_SIZE);

void start_state();
void charge_state();
void run_state();
void shut_state();
void alarm_state();
void update_leds();
void purge();

void contoller_event_queue_thread(){
  cont_queue.call(start_state);
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
  }
  else if (state == RUN_STATE){
    run_led.write(true);
  }
}

void start_state(){
  controller_flags.clear(CLEAR_EVENT_FLAG);
  controller_flags.set(START_EVENT_FLAG);
  fc.set_fc_status(START_STATE);
  // Signal fans to high
  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_MAX_FLAG);

  update_leds();

  // Close all valves, Open all relays
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  
  // Signal wait for fans
  controller_flags.wait_all(FAN_SPOOLED_FLAG);

  // Startup purge
  supply_v.write(true);
  Thread::wait(1000);
  purge_v.write(true);
  Thread::wait(200);
  purge_v.write(false);

  // Start resistors
  start_r.write(true);
  controller_flags.wait_all(START_RESISTOR_FLAG);
  start_r.write(false);
  Thread::wait(100);

  // Signal fans to minimum
  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_MIN_FLAG);
  controller_flags.set(FINISHED_EXCUTION_FLAG);
}

void charge_state(){
  // Sinal charge state
  controller_flags.clear(CLEAR_EVENT_FLAG);
  controller_flags.set(CHARGE_EVENT_FLAG);
  fc.set_fc_status(CHARGE_STATE);

  // For safety
  start_r.write(false);
  cap_r.write(false);

  // Charging
  controller_flags.wait_all(CHARGE_START_FLAG);
  charge_r.write(true);
  controller_flags.wait_all(CHARGE_STOP_FLAG);
  charge_r.write(false);


  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_MIN_FLAG);
  controller_flags.set(FINISHED_EXCUTION_FLAG);

}

void run_state(){
  controller_flags.clear(CLEAR_EVENT_FLAG);
  fc.set_fc_status(RUN_STATE);
  controller_flags.set(RUN_EVENT_FLAG);

  update_leds();
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(true);
  charge_r.write(false);
  cap_r.write(true);

  // Signal PID for fans
  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_PID_FLAG);
  controller_flags.set(FINISHED_EXCUTION_FLAG);
}

void shut_state(){
  fc.set_fc_status(SHUTDOWN_STATE);
  controller_flags.clear(CLEAR_EVENT_FLAG);
  controller_flags.set(SHUT_EVENT_FLAG);

  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_SHUTDOWN_FLAG);
  controller_flags.set(FINISHED_EXCUTION_FLAG);
}

void alarm_state(){
  fc.set_fc_status(ALARM_STATE);
  controller_flags.clear(CLEAR_EVENT_FLAG);
  controller_flags.set(ALARM_EVENT_FLAG);

  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);

  controller_flags.clear(CLEAR_FAN_FLAG);
  controller_flags.set(FAN_SHUTDOWN_FLAG);
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