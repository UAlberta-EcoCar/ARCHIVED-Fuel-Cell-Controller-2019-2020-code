#ifndef FAN_H
#define FAN_H

#include "PwmIn.h"

// Multi inheritance is bad, but whatever
class Fan: public PwmIn, public PwmOut{

public:

Fan(
  PinName out_pin,
  PinName in_pin
);
};

// TODO: methods for artifically setting values and stuff idk rn im tired


#endif
