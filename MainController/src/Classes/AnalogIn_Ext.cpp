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
  mu.lock();
  this->value = read();
  _scale();

  //TODO: Error Checking
  mu.unlock();
}

void AnalogIn_Ext::return_val(float &ref, bool update=0)
{
  if (update){
    update();
  }
  mu.lock();
  *ref = this->value;
  mu.unlock();
}

void AnalogIn_Ext::set_error_threshold(float error_threshold)
{
mu.lock();
this->error_threshold = error_threshold;
mu.unlock();
}

void AnalogIn_Ext::set_scale_type(bool type)
{
  mu.lock();
  this->type = type;
  mu.unlock();
}

void AnalogIn_Ext::set_scale_param(float a, float b)
{
  mu.lock();
  this->a = a;
  this->b = b;
  mu.unlock();
}

void AnalogIn_Ext::set_name(string name)
{
  mu.lock();
  this->name = name;
  mu.unlock();
}

void AnalogIn_Ext::set(float value, float int_value)
{
  mu.lock();
  this->value = value;
  this->int_value = int_value;
  mu.unlock();
}

void AnalogIn_Ext::_scale()
{
  mu.lock();
  switch (this->type) {
    case 0: //Linear
      this->value = this->a * this->value + this->b;
    case 1: // Exponential, TODO: Calculate if this will even fit in a float at maximum in
      this->value = this->a * exp(this->value) + this->b;
  }
  mu.unlock();
}
