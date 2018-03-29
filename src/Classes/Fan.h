#ifndef FAN_H
#define FAN_H

#include "PwmIn.h"
#include "IO.h"

class Fan: public IO{
  private:
    bool __coup;

    PwmOut out;

  public:

  static const float max;
  static const float min;
  static const float off;

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
