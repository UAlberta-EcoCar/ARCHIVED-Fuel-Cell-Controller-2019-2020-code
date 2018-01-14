#include <mbed.h>
#include <mbed_events.h>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"
#include "Classes/FuelCell.h"

// Defs
#include "Def/constants.h"
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"
#include "Def/semaphore_def.h"

#include "error_event_queue.h"

EventQueue cont_queue(32*EVENTS_EVENT_SIZE);

void start_state();
void run_state();
void shut_state();
void alarm_state();
void update_leds();

Event<void()> start_event(&cont_queue, start_state);
Event<void()> run_event(&cont_queue, run_state);
Event<void()> shutdown_event(&cont_queue, shut_state);

int contoller_event_queue_thread(){
  start_event.post();
  cont_queue.dispatch_forever();


  while(1){Thread::wait(10000);}
  return 1;
}

void update_leds(){
  alarm_led.write(false);
  shut_led.write(false);
  start_led.write(false);
  run_led.write(false);
  fc.lock();
  if (fc.get_fc_status() == ALARM_STATE){
    alarm_led.write(true);
  }
  else if (fc.get_fc_status() == SHUTDOWN_STATE){
    shut_led.write(true);
  }
  else if (fc.get_fc_status() == START_STATE){
    start_led.write(true);
  }
  else if (fc.get_fc_status() == RUN_STATE){
    run_led.write(true);
  }
  fc.unlock();
}

void start_state(){
  fc.set_fc_status(START_STATE);
  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  fcc_r.write(false);
  fan1.set_out(1.0);
  fan2.set_out(1.0);
  fan3.set_out(1.0);
  fan_spooled.wait();

  supply_v.write(true);
  purge_v.write(true);
  Thread::wait(1000);
  start_r.write(true);
  startup_purge.wait();
  purge_v.write(false);
  start_r.write(false);
  charge_r.write(true);
  // Start Fan control

  run_event.post();
}

void run_state(){
  fc.set_fc_status(RUN_STATE);
  update_leds();
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(true);
  charge_r.write(false);
  cap_r.write(true);
  fcc_r.write(true);
}

void shut_state(){
  fc.set_fc_status(SHUTDOWN_STATE);
  update_leds();
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  fcc_r.write(false);
  // Stop fans
}

void alarm_state(){
  fc.set_fc_status(ALARM_STATE);
  update_leds();
  while(true){
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  fcc_r.write(false);
  // Stop fans
  Thread::wait(100);
  }
  
}