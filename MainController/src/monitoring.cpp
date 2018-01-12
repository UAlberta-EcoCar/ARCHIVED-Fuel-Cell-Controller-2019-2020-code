#include <mbed.h>
#include <mbed_events.h>
#include <vector>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"

// Defs
#include "Def/constants.h"
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"
#include "Def/semaphore_def.h"

#include "fc_status.h"
#include "error_event_queue.h"
#include "controller_event_queue.h"

float ave_speed;
float ave_press;
Semaphore fan_spooled(0), startup_purge(0);
vector<Sensor*> pres_vec;
vector<Sensor*>::iterator pres_iter;

void monitoring_thread(){

  while(true){
    for (int_iter = int_vec.begin(); int_iter != int_vec.end(); int_iter++){
      (*(*int_iter)).update();
    }

    // Error Checking
    if (fcvolt.read() >= 0.75 || capvolt.read() >= 32.0){
      error_throw.write(false);
    }

    if (fccurr.read() >= 0.75 || capcurr.read() >= 70){
      error_throw.write(false);
    }

    if (press1.read() >= 8.0){
      error_throw.write(false);
    }

    if (fctemp1.read() >= 60.0 || fctemp2.read() >= 60.0){
      error_throw.write(false);
    }




    switch (get_fc_status()) {
      case START_STATE:
        fan1.lock();
        fan2.lock();
        fan3.lock();
        
        if(fan1.is_spooled() && fan2.is_spooled() && fan3.is_spooled()){
          fan_spooled.release();
        }
        fan1.unlock();
        fan2.unlock();
        fan3.unlock();
        fcvolt.lock();
        if(fcvolt.read() > FC_VOLT){
          startup_purge.release();
        }
        fcvolt.unlock();
                
      case RUN_STATE:
        ;
      case ALARM_STATE:
        // Temporary solution to a bug in which valves reopen.
        supply_v.write(false);
        purge_v.write(false);
        start_r.write(false);
        motor_r.write(false);
        charge_r.write(false);
        cap_r.write(false);
        fcc_r.write(false);
      default:;
    }
    Thread::wait(1);
  }

}
