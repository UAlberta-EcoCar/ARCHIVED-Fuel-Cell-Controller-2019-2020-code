#ifndef FAN_H
#define FAN_H

#include "PwmIn.h"
#include "IO.h"

class Fan: public IO{
static const float max;
static const float min;
bool __coup;
float __dutyIn;

PwmIn in;
PwmOut out;
public:

Fan(
  string name,
  PinName out_pin,
  PinName in_pin
);

// Getters/Setters
void set_out(float percentage);
void set_in(float percentage, bool couple=0);
float get_out();
float get_in();

// Other Methods
bool is_spooled();
void couple();
void decouple();
string toString();
string toStringInfo();

};



#endif
