#include "mbed.h"
#include "rtos.h"

#include <PwmOut.h>

#include <pin_defs.h>
#include <fan.h>
#include <analogs.h>
#include <fc_state_machine.h>

float fctemp;

// Object declarations
DigitalOut led1(LED1); //remove later
DigitalOut led3(LED3); //REMOVE LATER
PwmOut pwm_1(PWM_1);
DigitalOut fan_switch_all(FAN_SWITCH_ALL);

void fan_thread() {
    // Loop and check alert conditions at 10 Hz
    while (true) {
      fctemp = get_analog_values().fctemp1;
      // Set permanent fan period; adjust duty cycle only
      pwm_1.period(0.00004); // 40us fan period CHECK THIS

      // Choose fan behaviour based on current state
      // NOTE: pwm will only output if H2 STOP input signal is 3.3V
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
          led1.write(true);
          fan_switch_all.write(true);
          pwm_1.write(LOW);
          ThisThread::sleep_for(2000);
          led1.write(false);
          fan_switch_all.write(false);
          ThisThread::sleep_for(3000);
          fan_switch_all.write(true);
          ThisThread::sleep_for(3000);
          break;
        default :
          pwm_1.write(LOW);
      }

      ThisThread::sleep_for(1000);
    }
}
