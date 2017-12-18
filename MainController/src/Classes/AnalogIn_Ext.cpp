#include <mbed.h>
#include <limits>
#include <math.h>

#include "AnalogIn_Ext.h"

using namespace std;

// Constructor method
AnalogIn_Ext::AnalogIn_Ext(
  PinName pin,
  float error_threshold,
  string name,
  bool type,
  float a,
  float b
)
:AnalogIn(pin)
{
  this->name = name;
  this->error_threshold = error_threshold;
  this->type = type;
  this->a = a;
  this->b = b;
}

// Method Definition
void AnalogIn_Ext::update()
{
  _scale(read());
  if (this->value > error_threshold){
    //TODO: Error Checking
  }
  }


float AnalogIn_Ext::return_val(bool update)
{
  if (update){
    this->update();
  }
  return this->value;
}

void AnalogIn_Ext::set_error_threshold(float error_threshold)
{
this->error_threshold = error_threshold;
}

void AnalogIn_Ext::set_scale_type(bool type)
{
  this->type = type;
}

void AnalogIn_Ext::set_scale_param(float a, float b)
{
  this->a = a;
  this->b = b;
}

void AnalogIn_Ext::set_name(string name)
{
  this->name = name;
}

void AnalogIn_Ext::set(float value)
{
  this->value = value;
}

void AnalogIn_Ext::_scale(float val)
{
  switch (this->type) {
    case 0: //Linear
      this->value = this->a * val + this->b;
    case 1: // Exponential, TODO: Calculate if this will even fit in a float at maximum in
      this->value = this->a * exp(val) + this->b;
  }
}
