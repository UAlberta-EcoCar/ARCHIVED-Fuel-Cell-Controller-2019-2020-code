#include <mbed.h>
#include <rtos.h>
#include <mbed_events.h>

#include "private_lib/digital_io.h"
#include "private_lib/analog_io.h"
#include "private_lib/PwmIn.h"
#include "private_lib/pin_def.h"
#include "private_lib/fc_status.h"
#include "private_lib/fan_control.h"
#include "main.h"

Timer tim;
float dt;
float ave_speed;
float ave_press;
Semaphore fan_spooled(0), startup_purge(0);

void monitoring_thread(){

  tim.start();

  while(1){
    set_capvolt();
    set_fccurr();
    set_fcvolt();
    set_capcurr();
    //set_fcpres();

    tim.stop();
    dt = tim.read();
    set_fc_coulumbs(dt);
    set_fc_joules(dt);
    set_cap_coulumbs(dt);
    set_cap_joules(dt);
    tim.reset();

    if (get_capvolt() > 32.0 || get_fcvolt() > 30.8){
      throw_error();
      //TODO: add error code storage
    }

    if (get_fcpres() > 8){
      throw_error();
    }


    //TODO: Add error's for over curr, overtemp, under press, under voltage

    switch (get_fc_status()) {
      case START_STATE:
        ave_speed = (get_fan_1() + get_fan_2() +get_fan_3())/3;
        if (ave_speed >= get_fans() - 0.1 ){
          fan_spooled.release();
        }
      case RUN_STATE:
        ;
      default:;
    }
  }
}
