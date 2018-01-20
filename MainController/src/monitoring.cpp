#include <mbed.h>
#include <mbed_events.h>
#include <vector>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Sensor.h"
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
#include "controller_event_queue.h"
#include "main.h"

Semaphore fan_spooled(0), startup_purge(0);
EventFlags controller_flags;

Event<void()> shutdown_event(&cont_queue, shut_state);
Event<void()> start_event(&cont_queue, start_state);
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
        fc.set_error_status(1);
        error_isr();
      }

      if (fccurr.read() >= 70 || capcurr.read() >= 70){
        fc.set_error_status(2);
         error_isr();
      }

      if (press1.read() >= 8.0){
        fc.set_error_status(3);
        error_isr();
      }

      if (fctemp1.read() >= 60.0 || fctemp2.read() >= 60.0){
        fc.set_error_status(4);
        error_isr();
      }
    }
    
    // Code for digital in's that can't be covered with a interrupt
    // Button Code for start button
    if (start_butt.read() && (controller_flags.get()&0x20 == 0)){
      switch (fc.get_fc_status){
        case SHUTDOWN_STATE:
          start_event.post();
        default:
          shutdown_event.post();
      }
    }
    
    // Button Code for ol_rst
    // Button Code for hum_rst


    // Signal Checking
    uint32_t flags = controller_flags.get();
    // If no error and excution of last event has finished
    if (!(flags&0x20) && (flags&0x1)){

      if (flags&0x2){
        run_event.post();
      }
      
    }


    switch (fc.get_fc_status()) {
      case START_STATE:
        fan1.lock();
        fan2.lock();
        fan3.lock();
        
        if(fan1.is_spooled()){
          controller_flags.set(0x10000);
        }
        fan1.unlock();
        fan2.unlock();
        fan3.unlock();
        fcvolt.lock();
        if(fcvolt.read() > FC_VOLT){
          controller_flags.set(0x2000);
        }
        fcvolt.unlock();
                
      case RUN_STATE:
        fc.lock();
        if (fc_coulumbs.read()%((fc.get_num_purges)*2300) > 2300){
          purge_event.post();
        };
        fc.unlock();
      default:;
    }
    Thread::wait(1);
  }

}
