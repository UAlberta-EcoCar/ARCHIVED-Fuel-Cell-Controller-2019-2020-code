#include <mbed.h>
#include <limits>
#include <math.h>

#include "Analog_Sensor.h"

using namespace std;

// Constructor method
Analog_Sensor::Analog_Sensor(
  PinName pin,
  Scalable* scaler,
  string name
)
:Sensor(name), pin(pin)
{
  this->set_name(name);
  this->scaler = scaler;
  this->couple();
}

// Method Definition
void Analog_Sensor::update()
{
  this->lock();
  if (this->_coup){
    this->set((*(this->scaler)).scale(this->pin.read()));
  }
  this->unlock();
}


float Analog_Sensor::read(bool update)
{
  if (update){
    this->update();
  }
  return this->value;
}

void Analog_Sensor::set(float value, bool decouple)
{
  if (decouple){
    this->decouple();
  }
  this->lock();
  this->value = value;
  this->unlock();
}

void Analog_Sensor::couple()
{
  this->lock();
  this->_coup = 1;
  this->unlock();
}

void Analog_Sensor::decouple()
{
  this->lock();
  this->_coup = 0;
  this->unlock();
}
