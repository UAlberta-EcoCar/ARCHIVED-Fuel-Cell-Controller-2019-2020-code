#include <mbed.h>

#include "DigitalOut_Ext.h"

using namespace std;

DigitalOut_Ext::DigitalOut_Ext(
  PinName pin
)
:DigitalOut(pin)
{
  // Do Nothing
}

void DigitalOut_Ext::write_safe(
  int value
)
{
  mu.lock();
  write(value);
  mu.unlock();
}

int DigitalOut_Ext::read_safe()
{
  mu.lock();
  this->value = read();
  mu.unlock();

  return this->value;
}
