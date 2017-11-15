#include <mbed.h>
#include <rtos.h>
#include <mbed_events.h>

#include "private_lib/digital_io.h"
#include "private_lib/analog_io.h"
#include "private_lib/PwmIn.h"
#include "private_lib/pin_def.h"
#include "private_lib/fc_status.h"
#include "private_lib/fan_control.h"
#include "private_lib/signal_def.h"

Timer dt;

void monitoring_thread(){

  dt.start();

  while(1){

    //TODO: Everything

  }
}
