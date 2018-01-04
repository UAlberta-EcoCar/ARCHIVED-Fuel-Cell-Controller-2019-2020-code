#ifndef ANALOGIN_SENSOR_H
#define ANALOGIN_SENSOR_H

#include "Sensor.h"
#include "Scalable.h"
#include "LinearScalable.h"

using namespace std;

template <class T>
class Analog_Sensor: public Sensor{
private:
  float value;
  bool _coup;
  T scaler;
  AnalogIn pin;

/*
Method declaration
*/
public:
// Constructor
Analog_Sensor(
  PinName pin,
  T scaler,
  string name="default"
):Sensor(name), pin(pin)
{
  this->scaler = scaler;
  this->couple();
};

void update(){
  this->lock();
  if (this->_coup){
    this->set(this->scaler.scale(this->pin.read()));
  }
  this->unlock();
};

float read(bool update=0)
{
  if (update){
    this->update();
  }
  return this->value;
};

void set(float value, bool decouple=1)
{
  if (decouple){
    this->decouple();
  }
  this->lock();
  this->value = value;
  this->unlock();
};

void couple()
{
  this->lock();
  this->_coup = 1;
  this->unlock();
};

void decouple()
{
  this->lock();
  this->_coup = 0;
  this->unlock();
};

string toString()
{
  stringstream ss;
  ss << this->get_name();
  ss << ": ";
  ss << fixed << setprecision(2) << this->read();
  return ss.str();
};

string toStringInfo()
{
  stringstream ss;
  ss << fixed << setprecision(2) << this->read();
  return ss.str();

}


};

#endif
