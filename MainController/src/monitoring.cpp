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
#include "main.h"

EventFlags controller_flags;

Event<void()> shutdown_event(&cont_queue, shut_state);
Event<void()> start_event(&cont_queue, start_state);
Event<void()> run_event(&cont_queue, run_state);
Event<void()> purge_event(&cont_queue, purge);

DigitalIn start_butt(START, PullDown);
DigitalIn ol_rst(OL_RST, PullDown);
DigitalIn hum_rst(HUM_RST, PullDown);
DigitalIn button(BUTT, PullDown);

void monitoring_thread(){

  while(true){
    for (int_iter = int_vec.begin(); int_iter != int_vec.end(); int_iter++){
      (*(*int_iter)).update();
    }

    // Error Checking
    if (fc.get_error_status() == 0){
      if (fcvolt.read() >= 30.8 || capvolt.read() >= 32.0){
        fc.set_error_status(OVERVOLTAGE);
        error_isr();
      }

      if (fccurr.read() >= 70 || capcurr.read() >= 70){
        fc.set_error_status(OVERCURR);
        error_isr();
      }

      if (press1.read() >= 8.0){
        fc.set_error_status(OVERPRESS);
        error_isr();
      }

      if (press1.read() <= 5.0){
        fc.set_error_status(UNDERPRESS);
        error_isr();
      }

      fccurr.lock();
      float curr = fccurr.read();
      fccurr.unlock();

      if (fctemp1.read() >= fc.query_max_temp(curr) || fctemp2.read() >= fc.query_max_temp(curr)){
        fc.set_error_status(OVERTEMP);
        error_isr();
      }

      if (fctemp1.read() >= fc.query_min_temp(curr) || fctemp2.read() >= fc.query_min_temp(curr)){
        fc.set_error_status(UNDERTEMP);
        error_isr();
      }

    }

    uint32_t flags = controller_flags.get();
    // Code for digital in's that can't be covered with a interrupt
    // Button Code for start button
    if ((start_butt.read() == 1) && ((flags&ALARM_EVENT_FLAG) == 0)){
      switch (fc.get_fc_status()){
        case SHUTDOWN_STATE:
          start_event.post();
        default:
          shutdown_event.post();
      }
    }
    
    // Button Code for ol_rst
    // Button Code for hum_rst


    // Signal Checking
    // If no error and excution of last event has finished
    if (!(flags&ALARM_EVENT_FLAG) && (flags&FINISHED_EXCUTION_FLAG)){

      if (flags&START_EVENT_FLAG){
        run_event.post();
      }
      
    }
    // Fan Control
    switch(flags&CLEAR_FAN_FLAG){
      case FAN_SHUTDOWN_FLAG:
        fan_cont.set(Fan::off);
      case FAN_MIN_FLAG:
        fan_cont.set(Fan::min);
      case FAN_MAX_FLAG:
        fan_cont.set(Fan::max);
      case FAN_PID_FLAG:
        fan_cont.pid_update();
      default:;
    }


    switch (fc.get_fc_status()) {
      case START_STATE:

        fan1.lock();    
        if(fan1.is_spooled()){
          controller_flags.set(FAN_SPOOLED_FLAG);
        }
        fan1.unlock();

        fcvolt.lock();
        if(fcvolt.read() > FC_VOLT){
          controller_flags.set(START_PURGE_FLAG);
        }
        fcvolt.unlock();
                
      case RUN_STATE:
        fc.lock();
        fc_coulumbs.lock();
        if ((fc_coulumbs.read() - (float)(fc.get_num_purges()*2300)) > 2300){
          purge_event.post();
        };
        fc_coulumbs.unlock();
        fc.unlock();
      default:;
    }
    Thread::wait(1);
  }
}
