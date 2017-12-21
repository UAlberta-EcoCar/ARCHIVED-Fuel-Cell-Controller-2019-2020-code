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

  supply_v.write_safe(false);
  purge_v.write_safe(false);

  start_r.write_safe(false);
  motor_r.write_safe(false);
  charge_r.write_safe(false);
  cap_r.write_safe(false);
  fcc_r.write_safe(false);

  fan1.set_dutycycle(0.0);
  fan2.set_dutycycle(0.0);
  fan3.set_dutycycle(0.0);

  //fan_spooled.wait();
}
void start_purge(){
  supply_v.write_safe(true);
  purge_v.write_safe(false);

  start_r.write_safe(false);
  motor_r.write_safe(false);
  charge_r.write_safe(false);
  cap_r.write_safe(false);
  fcc_r.write_safe(false);

  //startup_purge.wait();

  start_r.write_safe(true);
  purge_v.write_safe(true);

  Thread::wait(1000);

  start_r.write_safe(false);
  purge_v.write_safe(false);

}

void start_end(){
    Thread::wait(1000);
  supply_v.write_safe(true);
  purge_v.write_safe(false);

  start_r.write_safe(false);
  motor_r.write_safe(false);
  charge_r.write_safe(false);
  cap_r.write_safe(true);
  fcc_r.write_safe(false);

  fan1.set_dutycycle(0.35);
  fan2.set_dutycycle(0.35);
  fan3.set_dutycycle(0.35);
}

void start_charge(){
    Thread::wait(1000);
  supply_v.write_safe(false);
  purge_v.write_safe(false);

  start_r.write_safe(false);
  motor_r.write_safe(false);
  charge_r.write_safe(true);
  cap_r.write_safe(false);
  fcc_r.write_safe(false);

  //cap_thres_l.wait();

  charge_r.write_safe(false);
  cap_r.write_safe(true);

  //cap_thres_h.wait();
}

void run_setup(){
  supply_v.write_safe(true);
  purge_v.write_safe(false);

  start_r.write_safe(false);
  motor_r.write_safe(true);
  charge_r.write_safe(false);
  cap_r.write_safe(true);
  fcc_r.write_safe(true); // FC relay isn't ever changed in old code?
}

void purge(){
  purge_v.write_safe(true);
  Thread::wait(200);
  purge_v.write_safe(true);
}

void shutdown_state(){
  supply_v.write_safe(false);
  purge_v.write_safe(false);

  start_r.write_safe(false);
  motor_r.write_safe(false);
  charge_r.write_safe(false);
  cap_r.write_safe(false);
  fcc_r.write_safe(false);

  fan1.set_dutycycle(0.0);
  fan2.set_dutycycle(0.0);
  fan3.set_dutycycle(0.0);

}

void test_state(){
  supply_v.write_safe(true);
  purge_v.write_safe(true);

  start_r.write_safe(true);
  motor_r.write_safe(true);
  charge_r.write_safe(true);
  cap_r.write_safe(true);
  fcc_r.write_safe(true);

}
