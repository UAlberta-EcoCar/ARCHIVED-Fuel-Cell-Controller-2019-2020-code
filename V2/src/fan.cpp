#include "mbed.h"
#include "rtos.h"

#include <PwmOut.h>

#include <pin_defs.h>
#include <fan.h>
#include <analogs.h>
#include <fc_state_machine.h>

float fctemp;

// Object declarations
PwmOut pwm_1(PWM_1);
DigitalOut fan_switch_all(FAN_SWITCH_ALL);

void fan_thread() {
    // Loop and check alert conditions at 10 Hz
    while (true) {
      fctemp = get_analog_values().fctemp1;
      // Set permanent fan period; adjust duty cycle only
      pwm_1.period(1); // 40us fan period CHECK THIS

      // Choose fan behaviour based on current state
      switch(get_fc_state()){
        case FC_INIT :
          fan_switch_all.write(false);
          break;
        case FC_STARTUP :
          fan_switch_all.write(true);
          pwm_1.write(LOW);
          break;
        case FC_FAULT :
          fan_switch_all.write(false);
          break;
        case FC_TEST :
          pwm_1.write(HIGH);
          fan_switch_all.write(true);
          ThisThread::sleep_for(2000);
          pwm_1.write(MEDIUM);
          ThisThread::sleep_for(3000);
          fan_switch_all.write(false);
          ThisThread::sleep_for(3000);
          fan_switch_all.write(true);
          ThisThread::sleep_for(2000);
          pwm_1.write(LOW);
          ThisThread::sleep_for(5000);
          pwm_1.write(MEDIUM);
          ThisThread::sleep_for(5000);
          pwm_1.write(HIGH);
          ThisThread::sleep_for(5000);
          break;
        default : fan_switch_all.write(false);
          pwm_1.write(LOW);
      }

      ThisThread::sleep_for(0.1);
    }
}
