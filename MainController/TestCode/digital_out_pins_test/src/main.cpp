#include <mbed.h>
#include "rtos.h"
#include "..\..\..\lib\pin_defines.h"

/*
This test code will test the digital_out pins to verify they are working
on a board

To test a specific pin place the pin name where indicated

Testing without output buffers
  -An output pin should draw less than 1 mA
    > a resistor, with a resistance higher than 3.3 kOhm (should be high
        as possible to reduce current draw)
  -Only one pin should be tested at a time
  -The scope should be used to meassure the output

Digital Output pins used on the board:
  FCC_RELAY
  CAP_RELAY
  MOTOR_RELAY
  CHARGE_RELAY
  STOP
  H2_OK
  ESTOP1
  ESTOP2
  TACH_1
  TACH_2
  TACH_3
  Valve1
  Valve2
  Valve3
  Valve4
  FAN_PWR
*/

DigitalOut test_pin(CAP_R);

int main() {
  int state = 0;

  while(true){
    test_pin.write(!state);
    state = !state;

    Thread::wait(500);
  }

  return 0;
}
