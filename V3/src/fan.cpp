#include "mbed.h"
#include "rtos.h"

#include <math.h>
#include <PwmOut.h>

#include <pin_defs.h>
#include <fan.h>
#include <analogs.h>
#include <fc_state_machine.h>

float fctemp;
float fccurrent;
float idealtemp;

// Object declarations
PwmOut pwm_1(PWM_1);
PwmOut pwm_2(PWM_2);
PwmOut pwm_3(PWM_3);

DigitalOut fan_switch_all(FAN_SWITCH_ALL);

void fan_thread() {
    // Loop and check alert conditions at 10 Hz
    while (true) {
      fctemp = get_analog_values().fctemp1;
      pwm_1.period(0.00004); // 40us fan period CHECK THIS
      pwm_2.period(0.00004);
      pwm_3.period(0.00004);

      // Choose fan behaviour based on current state
      // NOTE: pwm will only output if H2 STOP input signal is 3.3V
      switch(get_fc_state()){
        case FC_STANDBY :
          fan_switch_all.write(false);
          break;
        case FC_PRESSURIZE:
        case FC_START_UP_PURGE:
        case FC_CHARGE :
          fan_switch_all.write(true);
          pwm_1.write(LOW);
          break;
        case FC_RUN :
          idealtemp = 52.204*(1-exp(-0.010*fccurrent)+38.095);
          if (fctemp>=55){
            pwm_1.write(HIGH); 
            pwm_2.write(HIGH); 
            pwm_3.write(HIGH);
          }
          else if (fctemp>=50){
            pwm_1.write(MEDIUM);
            pwm_2.write(MEDIUM);
            pwm_3.write(MEDIUM);
          }
          else
          {
            pwm_1.write(LOW);
            pwm_2.write(LOW);
            pwm_3.write(LOW);
          }
/*
          else if (fctemp<=idealtemp){
            pwm_1.write(LOW);
            pwm_2.write(LOW);
            pwm_3.write(LOW);
          }
          else if (fctemp>idealtemp && fctemp<=idealtemp+5) {
            pwm_1.write(MEDIUM);
            pwm_2.write(MEDIUM);
            pwm_3.write(MEDIUM);
          }
          else if (fctemp>idealtemp+5){
            pwm_1.write(HIGH);
            pwm_2.write(HIGH);
            pwm_3.write(HIGH);
          }
*/
          break;
        case FC_SHUTDOWN :
          fan_switch_all.write(false);
          break;
        case FC_ALARM :
          fan_switch_all.write(false);
          break;
        case FC_TEST :
          fan_switch_all.write(true);
          pwm_1.write(LOW);
          ThisThread::sleep_for(2000);
          fan_switch_all.write(false);
          ThisThread::sleep_for(3000);
          fan_switch_all.write(true);
          ThisThread::sleep_for(3000);
          break;
        default :
          pwm_1.write(LOW);
          pwm_2.write(LOW);
          pwm_3.write(LOW);
      }

      ThisThread::sleep_for(0.2);
    }
}
