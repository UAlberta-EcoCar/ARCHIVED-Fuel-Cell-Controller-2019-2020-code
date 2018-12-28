#ifndef FAN_H
#define FAN_H

#include "PwmIn.h"
#include "IO.h"

class Fan: public IO{
  private:
    bool __coup; // Used as a conditional variable within Fan.cpp to indicate whether or not the fan is allowed to be set.

    PwmOut out; // Declares object of Mbed class PwmOut.

  public:

  // Constant declarations	
  static const float max;
  static const float min;
  static const float off;

  // Constructors
  Fan(string name, PinName out_pin, PinName in_pin);

  // Getters/Setters
  void set_out(float percentage);
  float get_out();

  // Other Methods
  void couple();
  void decouple();
  string toString();
  string toStringInfo();
  string toStringHeader();
  string toJsonObject();

};



#endif
