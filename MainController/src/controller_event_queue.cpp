#include <mbed.h>
#include <mbed_events.h>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"

// Defs
#include "Def/constants.h"
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"

#include "fc_status.h"

EventQueue cont_queue(32*EVENTS_EVENT_SIZE);

void push_function(functiontype func){
  cont_queue.call(func);
}

void fan_spool_up(){
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  fcc_r.write(false);
  fan1.set_out(0.0);
  fan2.set_out(0.0);
  fan3.set_out(0.0);
  //fan_spooled.wait();
}
void start_purge(){
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  fcc_r.write(false);
  //startup_purge.wait();
  start_r.write(true);
  purge_v.write(true);
  Thread::wait(10000);
  start_r.write(false);
  purge_v.write(false);

}

void start_end(){
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(true);
  fcc_r.write(false);
  fan1.set_out(0.35);
  fan2.set_out(0.35);
  fan3.set_out(0.35);
}

void start_charge(){
  Thread::wait(1000);
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(true);
  cap_r.write(false);
  fcc_r.write(false);
  //cap_thres_l.wait();
  charge_r.write(false);
  cap_r.write(true);
  //cap_thres_h.wait();
}

void run_setup(){
  supply_v.write(true);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(true);
  charge_r.write(false);
  cap_r.write(true);
  fcc_r.write(true); 
}

void purge(){
  purge_v.write(true);
  Thread::wait(200);
  purge_v.write(true);
}

void shutdown_state(){
  supply_v.write(false);
  purge_v.write(false);
  start_r.write(false);
  motor_r.write(false);
  charge_r.write(false);
  cap_r.write(false);
  fcc_r.write(false);
  fan1.set_out(0.0);
  fan2.set_out(0.0);
  fan3.set_out(0.0);

}

void test_state(){
  supply_v.write(true);
  purge_v.write(true);
  start_r.write(true);
  motor_r.write(true);
  charge_r.write(true);
  cap_r.write(true);
  fcc_r.write(true);
}

void test(){
  cont_queue.call(test_state);
}

void update_leds(){
  alarm_led.write(false);
  shut_led.write(false);
  start_led.write(false);
  run_led.write(false);

  if (get_fc_status() == ALARM_STATE){
    alarm_led.write(true);
  }
  else if (get_fc_status() == SHUTDOWN_STATE){
    shut_led.write(true);
  }
  else if (get_fc_status() == START_STATE){
    start_led.write(true);
  }
  else if (get_fc_status() == RUN_STATE){
    run_led.write(true);
  }
}

void run(){
  set_fc_status(RUN_STATE);
  update_leds();
  cont_queue.call(run_setup);
}

void startup(){
  set_fc_status(START_STATE);
  update_leds();
  cont_queue.call(fan_spool_up);
  cont_queue.call(start_purge);
  cont_queue.call(start_charge);
  cont_queue.call(start_end);
  cont_queue.call(run);
}

void shutdown(){
  set_fc_status(SHUTDOWN_STATE);
  update_leds();
  cont_queue.call(shutdown_state);
}

void alarm(){
  set_fc_status(ALARM_STATE);
  update_leds();
  cont_queue.call(shutdown_state);
}

int contoller_event_queue_thread(){
  cont_queue.call(startup);
  cont_queue.dispatch_forever();


  while(1){Thread::wait(10000);}
  return 1;
}
