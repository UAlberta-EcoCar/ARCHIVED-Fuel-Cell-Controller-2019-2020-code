#include <mbed.h>
#include <rtos.h>

#include "./lib/pin_defines.h"

/*
This test code will test the serial ports to verify they are working
on the board

To test a specific pin place the pin name where indicated

Testing without output buffers
  -An output pin should draw less than 1 mA
    > a resistor, with a resistance higher than 3.3 kOhm (should be high
        as possible to reduce current draw)
  -Only one pin should be tested at a time
  -The scope should be used to meassure the output

Serial ports used on the board:

  -OL_TX
  -OL_RX

  -FTDI_TX
  -FTDI_RX

  -BLUE_TX
  -BLUE_RX

  -MOTOR_TXs
  -MOTOR_RX
*/

Serial serial_test(/*Put the TX pin here*/, /*Put the RX pin here*/, baud=/*baud rate here*/);

void int main() {

  while(true){

    Thread::wait();
  }

  return 0;
}
