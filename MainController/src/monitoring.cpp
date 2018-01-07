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

float ave_speed;
float ave_press;
Semaphore fan_spooled(0), startup_purge(0);


void monitoring_thread(){

    for (int_iter = int_vec.begin(); int_iter != int_vec.end(); int_iter++){
      (*(*int_iter)).update();
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
      default:;
    }
}
