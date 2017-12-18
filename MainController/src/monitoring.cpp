#include <mbed.h>
#include <mbed_events.h>
#include <vector>

// Classes
#include "Classes/AnalogIn_Ext.h"
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
vector<AnalogIn_Ext*> an_in_vec;
vector<Integrator*> int_vec;
vector<AnalogIn_Ext*>::iterator an_iter;
vector<Integrator*>::iterator int_iter;

Timer dt;

void monitoring_thread(){
  // Place objects in vector
  an_in_vec.push_back(&capvolt);
  an_in_vec.push_back(&fccurr);
  an_in_vec.push_back(&fcvolt);
  an_in_vec.push_back(&capcurr);
  an_in_vec.push_back(&motorvolt);
  an_in_vec.push_back(&motorcurr);
  an_in_vec.push_back(&press1);
  an_in_vec.push_back(&press2);
  an_in_vec.push_back(&press3);
  an_in_vec.push_back(&press4);
  an_in_vec.push_back(&fctemp1);
  an_in_vec.push_back(&fctemp2);
  an_in_vec.push_back(&temp1);
  an_in_vec.push_back(&temp2);
  an_in_vec.push_back(&temp3);
  an_in_vec.push_back(&temp4);
  an_in_vec.push_back(&temp5);

  // Intergrators
  int_vec.push_back(&fc_coulumbs);
  int_vec.push_back(&fc_joules);
  int_vec.push_back(&cap_coulumbs);
  int_vec.push_back(&cap_joules);

  dt.start();

  while(true){
    // AnalogIn_Ext object updates
    // iterates through vector
    for (an_iter = an_in_vec.begin(); an_iter != an_in_vec.end(); an_iter++){
      (*(*an_iter)).update();
    }

    // Integrator object updates
    // iterates through vector
    dt.stop();
    for (int_iter = int_vec.begin(); int_iter != int_vec.end(); int_iter++){
      (*(*int_iter)).update(dt.read());
    }
    dt.reset();

    // Errors covered within classes

    switch (get_fc_status()) {
      case START_STATE:
        if(fan1.is_spooled() && fan2.is_spooled() && fan3.is_spooled()){
          fan_spooled.release();
        }
        if(fcvolt.return_val() > FC_VOLT){
          startup_purge.release();
        }
      case RUN_STATE:
        ;
      default:;
    }
  }
}
