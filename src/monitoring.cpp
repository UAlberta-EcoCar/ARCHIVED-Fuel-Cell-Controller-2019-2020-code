#include <mbed.h>
#include <mbed_events.h>
#include <vector>

#include "Def/pin_def.h"
#include "Def/object_def.h"

#include "Classes/SerialPrinter.h"

#include "controller_event_queue.h"
#include "monitoring.h"

#ifndef ALICE_CONFIGURATION
#define FC_VOLT 20.0
#define CAP_VOLT 20.0
#endif

#ifdef ALICE_CONFIGURATION
#define FC_VOLT 30.0
#define CAP_VOLT 30.0
#endif

#define PURGE_COLUMBS 2300

EventFlags controller_flags;
EventQueue mon_queue;

Timer start_button_timer;
Timer button_timer;

// Function definitions
void update_integrators();
void fan_control();
void start_button();
void state_monitoring();

void start_purge_check();
void fc_charge_exit_check();
void cap_charge_entry_check();
void cap_charge_exit_check();
void purge_entry_check();

#ifdef ENABLE_RELAY_TEST
void relay_delay(){
  Thread::wait(2000);
}
#endif

// Controller events
Event<void()> shutdown_event(&cont_queue, shut_state);

// Start up events
Event<void()> start_event(&cont_queue, start_state);
Event<void()> start_purge_event(&cont_queue, start_purge);
Event<void()> fc_charge_exit_event(&cont_queue, fc_charge_exit);

// Charge events
Event<void()> charge_event(&cont_queue, charge_state);
Event<void()> cap_charge_entry_event(&cont_queue, cap_charge_entry);
Event<void()> cap_charge_exit_event(&cont_queue, cap_charge_exit);

// Run events
Event<void()> run_event(&cont_queue, run_state);
Event<void()> purge_event(&cont_queue, purge);

// Testing events
#ifdef ENABLE_RELAY_TEST
Event<void()> relay_delay_event(&cont_queue, relay_delay);
#endif

// Monitoring events
Event<void()> update_integrators_event(&mon_queue, update_integrators);
Event<void()> fan_control_event(&mon_queue, fan_control);
Event<void()> start_button_event(&mon_queue, start_button);


// State Monitoring events
Event<void()> state_monitoring_event(&mon_queue, state_monitoring);
// Start
Event<void()> start_purge_check_event(&mon_queue, start_purge_check);
Event<void()> fc_charge_exit_check_event(&mon_queue, fc_charge_exit_check);
//Charge
Event<void()> cap_charge_entry_check_event(&mon_queue, cap_charge_entry_check);
Event<void()> cap_charge_exit_check_event(&mon_queue, cap_charge_exit_check);
//Run
Event<void()> purge_entry_check_event(&mon_queue, purge_entry_check);

#ifdef ENABLE_EXTERNAL_START
InterruptIn start_butt(START);
#endif
InterruptIn button(BUTT);

// BUTTON ISR'S
void start_button_rise(){
  debug_led.write(true);
  start_button_timer.start();
}

void start_button_fall(){
  if (start_button_timer.read_ms() > 20){
    start_button_event.post();
  }    

  start_button_timer.stop();
  start_button_timer.reset();
  debug_led.write(false);
}

void button_rise(){
  debug_led.write(true);
  button_timer.start();
}

void button_fall(){
  if (button_timer.read_ms() > 20){
    start_button_event.post();
  }
  button_timer.stop();
  button_timer.reset();
  debug_led.write(false);
}


// EVENTS
void update_integrators(){
  for (int_iter = int_vec.begin(); int_iter != int_vec.end(); int_iter++){
      (*(*int_iter)).update();
    }
}

void fan_control(){
  uint32_t flags = controller_flags.get();
  if (flags&FAN_SHUTDOWN_FLAG){
    fan_cont.set(Fan::off);
  }
  else if (flags&FAN_MIN_FLAG){
    fan_cont.set(Fan::min);
  }
  else if (flags&FAN_MAX_FLAG){
    fan_cont.set(Fan::max); 
  }
  else if (flags&FAN_PID_FLAG){
    fan_cont.pid_update();
  }
}

void start_button(){
  fc.lock();
  int status = fc.get_fc_status();
  fc.unlock();

  if (status == SHUTDOWN_STATE){
    start_event.post();
  }
  else if (status != ALARM_STATE){
    shutdown_event.post();
  }
}

// STATE EVENTS
// Start State
void start_purge_check(){
  #ifdef ENABLE_RELAY_TEST
  if (true){
    relay_delay_event.post();
    start_purge_event.post();
    return;
  }
  #endif
  #ifndef ENABLE_RELAY_TEST
  fcvolt.lock();
  float volt = fcvolt.read();
  fcvolt.unlock();

  if (volt > FC_VOLT){
    start_purge_event.post();
    return;
  }
  #endif

  state_monitoring_event.delay(50);
  state_monitoring_event.post();
}

void fc_charge_exit_check(){
  #ifdef ENABLE_RELAY_TEST
  if (true){
    relay_delay_event.post();
    fc_charge_exit_event.post();
    return;
  }
  #endif
  #ifndef ENABLE_RELAY_TEST
  fcvolt.lock();
  float volt = fcvolt.read();
  fcvolt.unlock();

  if (volt > FC_VOLT){
    fc_charge_exit_event.post();
    return;
  }
  #endif
  
  state_monitoring_event.delay(50);
  state_monitoring_event.post();
}

// Charge State
void cap_charge_entry_check(){
  #ifndef ENABLE_RELAY_TEST
  capvolt.lock();
  float volt = capvolt.read();
  capvolt.unlock();

  // If voltage already acheived skip to after charge
  if (volt > CAP_VOLT){
    cap_charge_exit_event.post();
    return;
  }
  #endif
  cap_charge_entry_event.post();
}

void cap_charge_exit_check(){
  #ifdef ENABLE_RELAY_TEST
  if (true){
    relay_delay_event.post();
    cap_charge_exit_event.post();
    return;
  }
  #endif
  #ifndef ENABLE_RELAY_TEST
  capvolt.lock();
  float volt = capvolt.read();
  capvolt.unlock();

  if (volt > CAP_VOLT){
    cap_charge_exit_event.post();
    return;
  }
  #endif
  
  state_monitoring_event.delay(50);
  state_monitoring_event.post();
}

// Run State
void purge_entry_check(){

  #ifdef ENABLE_RELAY_TEST
  if (true){
    relay_delay_event.post();
    purge_event.post();
  }
  #endif
  #ifndef ENABLE_RELAY_TEST
  fc.lock();
  int num_purge = fc.get_num_purges();
  fc.unlock();
      
  fc_coulumbs.lock();
  float coulumbs = fc_coulumbs.read();
  fc_coulumbs.unlock();

  if ((coulumbs - (float)(num_purge*PURGE_COLUMBS)) > PURGE_COLUMBS){
    purge_event.post();
    return;
  }
  #endif

  state_monitoring_event.delay(50);
  state_monitoring_event.post();
}

void start_state_flags(uint32_t flags){
  if (flags&SIGNAL_STARTSETUPCOMPLETE){
    start_purge_check_event.post();
    return;
  }

  if (flags&SIGNAL_STARTPURGECOMPLETE){
    fc_charge_exit_check_event.post();
    return;
  }

  if (flags&SIGNAL_STATETRANSITION){
    charge_event.post();
    return;
  }

  state_monitoring_event.delay(50);
  state_monitoring_event.post();
}

void charge_state_flags(uint32_t flags){
  if (flags&SIGNAL_CHARGESETUPCOMPLETE){
    cap_charge_entry_check_event.post();
    return;
  }
  if (flags&SIGNAL_CHARGESTARTED){
    cap_charge_exit_check_event.post();
    return;
  }
  if (flags&SIGNAL_STATETRANSITION){
    run_event.post();
    return;
  }
  state_monitoring_event.delay(50);
  state_monitoring_event.post();
}

void purge_state_flags(uint32_t flags){
  if (flags&SIGNAL_STATETRANSITION){
    run_event.post();
    return;
  }
  state_monitoring_event.delay(50);
  state_monitoring_event.post();
}

void state_monitoring(){
  fc.lock();
  int status = fc.get_fc_status();
  fc.unlock();
  fan_control();

  uint32_t flags = controller_flags.get();
  if (status == START_STATE){
    start_state_flags(flags);
  }
  else if (status == CHARGE_STATE){
    charge_state_flags(flags);
  }
  else if (status == RUN_STATE){
    purge_entry_check_event.post();
  }
  else if (status == PURGE_STATE){
    purge_state_flags(flags);
  }
  else if (status == SHUTDOWN_STATE){
    #ifdef ENABLE_RELAY_TEST
    relay_delay_event.post();
    start_event.post();
    #endif
  }
}

void monitoring_thread(){
  // Button Setup

  button.rise(&button_rise);
  button.fall(&button_fall);

  #ifdef ENABLE_EXTERNAL_START
  start_butt.rise(&start_button_rise);
  start_butt.fall(&start_button_fall);
  start_butt.mode(PullDown);
  #endif

  button.mode(PullDown);



  update_integrators_event.period(10);
  //fan_control_event.period(100);

  update_integrators_event.post();
  //fan_control_event.post();
  while(true){
    mon_queue.dispatch(-1);
  }
}
