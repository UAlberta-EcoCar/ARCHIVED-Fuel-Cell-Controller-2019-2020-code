#ifndef ANALOGIN_EXT_H
#define ANALOGIN_EXT_H

//NOTE: Most everything in private_lib will be converted to classes to make
//      FTDI debugging a thing
//        -_def files will not be, I mean they just define values.

class AnalogIn_Ext{
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
float value;
float int_value;
float error_threshold;
float int_error_theashold;
bool int_flag;
bool int_started;

//TODO: Figure out why the f*** strings aren't "a valid type found in std"
//std::string name;

bool type;
float a;
float b;

Timer dt;
AnalogIn in;
Mutex mu;


/*
Method declaration
*/
public:
// Constructor
AnalogIn_Ext(
            PinName pin,
            bool int_flag=0,
            float error_threshold=2000000000.0,
            float int_error_threshold=2000000000.0,
            //std::string name = "default",
            bool type=0,
            float a=1,
            float b=0
          );

//TODO: Clean up doc below this
/*
Updates the values
*/

// These need work in designing the best way to do this ie returning whats needed
void update();
float update_and_return_value(bool update=1);
float update_and_return_int_value(bool update=1);

/*
Set's the integration flag
*/
void set_int_flag(bool int_flag);

/*
Set's the conversion functions parameters
*/
void set_conv_param(float a, float b);

/*
Artifically "set" the pin value/int value
  -Useful for test code
  -Changes the value stored in the objects data members
*/
void set(float value, float int_value);

private:

float _scale();
};

#endif
