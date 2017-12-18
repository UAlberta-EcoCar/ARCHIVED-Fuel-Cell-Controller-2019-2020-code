#ifndef ANALOGIN_EXT_H
#define ANALOGIN_EXT_H

#include <string> // Lol i guess you can include in headers )_)
#include <mbed.h>

//NOTE: Most everything in private_lib will be converted to classes to make
//      FTDI debugging a thing
//        *_def files will not be, I mean they just define values.

class AnalogIn_Ext: public AnalogIn{
/* Data Members
Blocks:
  -pin values and integration flag
    > AnalogIn readings and Integration value.
    >Flag determines if integration is to be applied.
  -name
    > Device name.
      ex. Thermistor 1
  -device type and parameters (linear or exponential)
    > Describes the conversion from analog in to real world values.
    > Linear:
      * ax + b
    > Exponential:
      *ae^x + b
      ex. volt in -> degrees celcius or volt in -> actual voltage
  -timer object
    > used only for integration, not externally accessible.
  -AnalogIn object
*/



/*
Method declaration
*/
public:
// Constructor
AnalogIn_Ext(
  PinName pin,
  float error_threshold=2000000000.0,
  std::string name = "default",
  bool type=0,
  float a=1,
  float b=0
);

//TODO: Clean up doc below this
/*
Updates the values
*/
/*
Set's the conversion functions parameters
*/
/*
Artifically "set" the pin value/int value
  -Useful for test code
  -Changes the value stored in the objects data members
*/
void update();
float return_val(bool update=0);
void set_error_threshold(float error_threshold);
void set_scale_type(bool type);
void set_scale_param(float a, float b);
void set_name(std::string name);
void set(float value);

protected:
  float value;
  float error_threshold;
  std::string name;
  bool type;
  float a;
  float b;

  Mutex mu;



void _scale(float val);
};

#endif
