#ifndef FAN_H
#define FAN_H

#include "PwmIn.h"

// Multi inheritance is bad, but whatever
// Private inheritance means the functions inherited can only be used internally
class Fan: protected PwmIn, protected PwmOut{

bool __lock;
float __dutyIn;

Mutex mu;
public:

Fan(
  PinName out_pin,
  PinName in_pin
);

// TODO: methods for artifically setting values and stuff idk rn im tired
void set_dutycycle(float percentage);
void set_in_dutycycle(float percentage, bool lock=0);

float get_dutycycle();
float get_in_dutycycle();

bool is_spooled();

// Lock prevents updating of the PwmIn, used for debugging.
void unlock();
void lock();

};



#endif
