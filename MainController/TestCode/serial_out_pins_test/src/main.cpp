#include <mbed.h>
#include <mbed_events.h>

#include "IO.h"
#include "SerialPrinter.h"
#include "pin_def.h"

SerialPrinter serial_test("Printer", OL_TX, OL_RX, 19200);
IO t1("Name1");
IO t2("Name2");
std::vector<IO*> vec;
std::vector<IO*>::iterator iter;


int main() {
  vec.push_back(&t1);
  vec.push_back(&t2);

  while(true){
    serial_test.print<IO>(&vec, &iter);
  }

  return 0;
}
