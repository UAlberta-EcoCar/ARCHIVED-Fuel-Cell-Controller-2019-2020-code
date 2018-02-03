#include <mbed.h>
#include <mbed_events.h>
#include <vector>

#include "Classes/Fan.h"
#include "Classes/Integrator.h"
#include "Classes/FuelCell.h"
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/constants.h"
#include "controller_event_queue.h"
#include "monitoring.h"

#define FC_PRES1 5.0f
#define FC_PRES2 4.0f
#define FC_VOLT 20.0f

EventFlags controller_flags;
EventQueue mon_queue;

Timer start_button_timer;

// Function definitions
void update_integrators();
void fan_control();
void state_control();
void state_monitoring();

// Controller events
Event<void()> shutdown_event(&cont_queue, shut_state);
Event<void()> start_event(&cont_queue, start_state);
Event<void()> run_event(&cont_queue, run_state);
Event<void()> purge_event(&cont_queue, purge);

// Monitoring events
Event<void()> update_integrators_event(&mon_queue, update_integrators);
Event<void()> fan_control_event(&mon_queue, fan_control);
Event<void()> state_control_event(&mon_queue, state_control);
Event<void()> state_monitoring_event(&mon_queue, state_monitoring);

DigitalIn start_butt(START, PullDown);
DigitalIn ol_rst(OL_RST, PullDown);
DigitalIn hum_rst(HUM_RST, PullDown);
DigitalIn button(BUTT, PullDown);

bool start_butt_prev = 0;

void update_integrators(){
  for (int_iter = int_vec.begin(); int_iter != int_vec.end(); int_iter++){
      (*(*int_iter)).update();
    }
}

void button_scanning(){
  uint32_t flags = controller_flags.get();

  if ((start_butt.read() == 1) && ((flags&ALARM_EVENT_FLAG) == 0)){
    int time_passed = start_button_timer.read_ms();
    
    if (!start_butt_prev && (time_passed > 500)){
      controller_flags.set(START_BUTTON_PRESSED);
      start_button_timer.reset();
      start_button_timer.start();
    }

    if (time_passed > 1000){
      start_button_timer.stop();
    }
  }
}

void state_control(){
  uint32_t flags = controller_flags.get();
  fc.lock();
  int status = fc.get_fc_status();
  fc.unlock();
  
  if (!(flags&ALARM_EVENT_FLAG) && (flags&FINISHED_EXCUTION_FLAG)){
    if (flags&START_BUTTON_PRESSED){
      controller_flags.clear(START_BUTTON_PRESSED);
      switch (status){
        case SHUTDOWN_STATE:
          start_event.post();
        default:
          shutdown_event.post();
      }
      controller_flags.clear(START_BUTTON_PRESSED);
    }
    else if (flags&START_EVENT_FLAG){
      run_event.post();
    }
  }
}

void state_monitoring(){
  fc.lock();
  int status = fc.get_fc_status();
  fc.unlock();

  switch (status) {
    case START_STATE:{
        fan1.lock(); 
        bool fan = fan1.is_spooled();
        fan1.unlock();

        fcvolt.lock();
        float volt = fcvolt.read();
        fcvolt.unlock();
  
        if(fan){
          controller_flags.set(FAN_SPOOLED_FLAG);
        }

        if(volt > FC_VOLT){
          controller_flags.set(START_PURGE_FLAG);
        }
      }

    case RUN_STATE:{
        fc.lock();
        int num_purge = fc.get_num_purges();
        fc.unlock();
      
        fc_coulumbs.lock();
        float coulumbs = fc_coulumbs.read();
        fc_coulumbs.unlock();

        if ((coulumbs - (float)(num_purge*2300)) > 2300){
          purge_event.post();
        }
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
  update_integrators_event.period(10);
  fan_control_event.period(100);
  state_control_event.period(100);
  state_monitoring_event.period(100);

  update_integrators_event.post();
  fan_control_event.post();
  state_control_event.post();
  state_monitoring_event.post();
  mon_queue.dispatch_forever();
}
