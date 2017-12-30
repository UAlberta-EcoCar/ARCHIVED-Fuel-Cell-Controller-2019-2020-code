#ifndef DIGITALOUT_EXT_H
#define DIGITALOUT_EXT_H

#include "IO.h"
#include <string>

// Class is used to make DigitalOut thread safe
class DigitalOut_Ext: public IO{

int value;
DigitalOut pin;

public:
// Constructor
DigitalOut_Ext(
  PinName pin,
  string name
):IO(name), pin(pin){};

// Method declaration
void write(
  int value
)
{
  this->lock();
  this->value = value;
  this->pin.write(value);
  this->unlock();
};

int read(){
  return this->value;
};
};
#endif
