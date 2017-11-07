#include <mbed.h>
#include <rtos.h>

#include "../../../lib/pin_defines.h"

/*
This test code will test the pwm_out pins to verify they are working
on a board

To test a specific pin place the pin name where indicated

Testing without output buffers
  -An output pin should draw less than 1 mA
    > a resistor, with a resistance higher than 3.3 kOhm (should be high
        as possible to reduce current draw)
  -Only one pin should be tested at a time
  -The scope should be used to meassure the output

PWM Output pins used on the board:
  -PWM_1
  -PWM_2
  -PWM_3
*/

PwmOut test_pin(PWM_1);

int main() {
  float duty_cycle = 0.5;
  int period = 20;

  test_pin.write(duty_cycle);
  test_pin.period_ms(period);

  while(true){

  }

  return 0;
}
