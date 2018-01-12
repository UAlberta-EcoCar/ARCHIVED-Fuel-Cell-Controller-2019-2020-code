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
#include "Def/semaphore_def.h"

#include "monitoring.h"

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
    Thread::wait(1000);
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
  fcc_r.write(true); // FC relay isn't ever changed in old code?
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
