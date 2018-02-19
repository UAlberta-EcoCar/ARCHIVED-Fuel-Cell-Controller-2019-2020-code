#include <mbed.h>
#include <mbed_events.h>
#include <vector>

#include "Def/pin_def.h"
#include "Def/object_def.h"

#include "controller_event_queue.h"
#include "monitoring.h"

#define FC_PRES1 5.0
#define FC_PRES2 4.0

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

// Function definitions
void update_integrators();
void fan_control();
void state_monitoring();
void button_scanning();

#ifdef ENABLE_RELAY_TEST
void relay_delay(){
  Thread::wait(1000);
}
#endif

// Controller events
Event<void()> shutdown_event(&cont_queue, shut_state);

// Start up events
Event<void()> start_event(&cont_queue, start_state);
Event<void()> start_purge_event(&cont_queue, start_purge);
Event<void()> start_res_start_event(&cont_queue, start_res_start);
Event<void()> start_res_end_event(&cont_queue, start_res_end);

// Charge events
Event<void()> charge_event(&cont_queue, charge_state);
Event<void()> charge_start_event(&cont_queue, charge_start);
Event<void()> charge_stop_event(&cont_queue, charge_stop);

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
Event<void()> state_monitoring_event(&mon_queue, state_monitoring);

#ifdef ENABLE_EXTERNAL_START
InterruptIn start_butt(START);
#endif
InterruptIn button(BUTT);

// BUTTON ISR'S
void start_button_rise(){
  start_button_timer.start();
}

void start_button_fall(){
  if (start_button_timer.read() > 20){
    switch(fc.get_fc_status()){
      case SHUTDOWN_STATE:{
        start_event.post();
      }
      default:{
        shutdown_event.post();
      }
    }
  }
  start_button_timer.stop();
  start_button_timer.reset();
}


// EVENTS
void update_integrators(){
  for (int_iter = int_vec.begin(); int_iter != int_vec.end(); int_iter++){
      (*(*int_iter)).update();
    }
}

void state_monitoring(){
  fc.lock();
  int status = fc.get_fc_status();
  fc.unlock();

  uint32_t flags = controller_flags.get();

  switch (status) {
    case START_STATE:{
        if (flags&SIGNAL_STARTSETUPCOMPLETE){
          #ifdef ENABLE_RELAY_TEST
          controller_flags.clear(SIGNAL_STARTSETUPCOMPLETE);
          relay_delay_event.post();
          start_purge_event.post();
          return;
          #endif

          fan1.lock(); 
          bool fan = fan1.is_spooled();
          fan1.unlock();

          if (fan){
            controller_flags.clear(SIGNAL_STARTSETUPCOMPLETE);
            start_purge_event.post();
            return;
          }
        }

        if (flags&SIGNAL_STARTPURGECOMPLETE){
          controller_flags.clear(SIGNAL_STARTPURGECOMPLETE);
          start_res_start_event.post();
          #ifdef ENABLE_RELAY_TEST
          relay_delay_event.post();
          #endif 
          return;
        }

        if (flags&SIGNAL_STARTRESSTARTED){
          #ifdef ENABLE_RELAY_TEST
          controller_flags.clear(SIGNAL_STARTRESSTARTED);
          relay_delay_event.post();
          start_res_end_event.post();
          return;
          #endif

          fcvolt.lock();
          float volt = fcvolt.read();
          fcvolt.unlock();

          if (volt > FC_VOLT){
            controller_flags.clear(SIGNAL_STARTRESSTARTED);
            start_res_end_event.post();
            return;
          }
        }
        if (flags&SIGNAL_STARTRESCOMPLETE){
          controller_flags.clear(SIGNAL_STARTRESCOMPLETE);
          #ifdef ENABLE_RELAY_TEST
          relay_delay_event.post();
          #endif
          charge_event.post();
          return;
        }
      }

    case CHARGE_STATE:{
        
        if (flags&SIGNAL_CHARGESETUPCOMPLETE){

          #ifdef ENABLE_RELAY_TEST
          controller_flags.clear(SIGNAL_CHARGESETUPCOMPLETE);
          relay_delay_event.post();
          charge_start_event.post();
          return;
          #endif

          capvolt.lock();
          float volt = capvolt.read();
          capvolt.unlock();

          if (volt < CAP_VOLT){
            controller_flags.clear(SIGNAL_CHARGESETUPCOMPLETE);
            charge_start_event.post();
            return;
          }
        }
        
        if (flags&SIGNAL_CHARGESTARTED){
          #ifdef ENABLE_RELAY_TEST
          controller_flags.clear(SIGNAL_CHARGESTARTED);
          relay_delay_event.post();
          charge_stop_event.post();
          return;
          #endif

          capvolt.lock();
          float volt = capvolt.read();
          capvolt.unlock();

          if (volt > CAP_VOLT){
            controller_flags.clear(SIGNAL_CHARGESTARTED);
            charge_stop_event.post();
            return;
          }
        }

        if (flags&SIGNAL_CHARGECOMPLETED){
          controller_flags.clear(SIGNAL_CHARGECOMPLETED);
          #ifdef ENABLE_RELAY_TEST
          relay_delay_event.post();
          #endif
          run_event.post();
          return;
        }
    }

    case RUN_STATE:{
        #ifdef ENABLE_RELAY_TEST
        relay_delay_event.post();
        purge_event.post();
        relay_delay_event.post();
        shutdown_event.post();
        return;
        #endif

        fc.lock();
        int num_purge = fc.get_num_purges();
        fc.unlock();
      
        fc_coulumbs.lock();
        float coulumbs = fc_coulumbs.read();
        fc_coulumbs.unlock();

        if ((coulumbs - (float)(num_purge*PURGE_COLUMBS)) > PURGE_COLUMBS){
          purge_event.post();
        }
      }

    case SHUTDOWN_STATE:{
      #ifdef ENABLE_RELAY_TEST
      relay_delay_event.post();
      start_event.post();
      #endif
    }
    default:;
  }
}

void fan_control(){
  uint32_t flags = controller_flags.get();
  switch((flags)&CLEAR_FAN_FLAG){
    case FAN_SHUTDOWN_FLAG:
      fan_cont.set(Fan::off);
    case FAN_MIN_FLAG:
      fan_cont.set(Fan::min);
    case FAN_MAX_FLAG:
      fan_cont.set(Fan::max);
    case FAN_PID_FLAG:
      fan_cont.pid_update();
  }
}

void monitoring_thread(){
  // Button Setup
  #ifdef ENABLE_EXTERNAL_START
  start_butt.rise(&start_button_rise);
  start_butt.fall(&start_button_fall);
  start_butt.mode(PullDown);
  #endif

  button.rise(&start_button_rise);
  button.fall(&start_button_fall);
  button.mode(PullDown);



  update_integrators_event.period(10);
  fan_control_event.period(100);
  state_monitoring_event.period(100);

  update_integrators_event.post();
  fan_control_event.post();
  state_monitoring_event.post();
  mon_queue.dispatch_forever();
}
