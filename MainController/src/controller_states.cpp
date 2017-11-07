#include <mbed.h>
#include <rtos.h>
#include <mbed_events.h>

#include "private_lib/digital_io.h"
#include "private_lib/pin_def.h"
#include "private_lib/signal_def.h"
#include "private_lib/fan_control.h"

//TODO: monitoring signals need to be changed once monitoring is added

void standby_state(){
  set_supply_valve(false); //closed
  set_purge_valve(false); //closed
  set_start_relay(false); // open
  set_fcc_relay(false); // open
  set_charge_relay(false); //open

  set_fans(0.0); // set fans speed to %0
  state_fans(false); // switch fans off
}

void fan_spool_up(){
  set_supply_valve(false); //closed
  set_purge_valve(false); //closed
  set_start_relay(false); //open
  set_fcc_relay(false); //open
  set_charge_relay(false);

  set_fans(1.0); // set fans speed to %100
  state_fans(true); // switch fans on

  //TODO: wait for fans to be on
  // wait from a signal from monitoring thread.
  Thread::signal_wait(0x1);
}
void start_purge(){
  set_supply_valve(true); // open
  set_purge_valve(false); // closed
  set_start_relay(false); // closed
  set_fcc_relay(false); // open
  set_charge_relay(false);

  Thread::signal_wait(0x1);

  set_start_relay(false); // closed
  set_purge_valve(true); // open

  Thread::wait(1000);
}

void start_end(){
  set_purge_valve(false);
  set_supply_valve(true);
  set_fcc_relay(true);
  set_start_relay(false);
  set_charge_relay(false);

  set_fans(0.35);
  state_fans(true);
}

void start_charge(){
  set_charge_relay(true);
  Thread::signal_wait(0X1);
  set_charge_relay(false);
  Thread::wait(500);
  set_charge_relay(true);
  Thread::signal_wait(0x1);
}

void run_setup(){
  set_purge_valve(false);
  set_supply_valve(true);
  set_fcc_relay(true);
  set_start_relay(false);
  set_charge_relay(false);

  set_fans(0.3);
  state_fans(true);
}

void purge(){
  set_purge_valve(true);
  Thread::wait(200);
  set_purge_valve(false);
}

void shut_down_state(){
  set_purge_valve(false);
  set_supply_valve(false);
  set_fcc_relay(false);
  set_start_relay(false);
  set_charge_relay(false);

  set_fans(0.0);
  state_fans(false);
}
