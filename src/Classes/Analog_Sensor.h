#ifndef ANALOGIN_SENSOR_H
#define ANALOGIN_SENSOR_H

#include "Sensor.h"
#include "Scalable.h"

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
  this->value = 0.0;
  this->scaler = scaler;
  this->couple();
};

void update(){
  
};

float read()
{
  this->lock();
  if (this->_coup){
    this->set(this->scaler.scale(this->pin.read()));
  }
  this->unlock();
  
  return this->value;
};

void set(float value, bool decouple=0){
  if (decouple){
    this->decouple();
  }
  this->lock();
  this->value = value;
  this->unlock();
};

void couple(){
  this->lock();
  this->_coup = 1;
  this->unlock();
};

void decouple(){
  this->lock();
  this->_coup = 0;
  this->unlock();
};

string toString(){
  stringstream ss;
  ss << this->get_name();
  ss << ":";
  ss << fixed << setprecision(2) << this->read();
  return ss.str();
};

string toStringInfo(){
  stringstream ss;
  ss << fixed << setprecision(2) << this->read();
  return ss.str();
};

string toStringHeader(){
  return this->get_name();
};

string toJsonObject(){
  stringstream ss;
  return "Not Implemented";
};
};

#endif
