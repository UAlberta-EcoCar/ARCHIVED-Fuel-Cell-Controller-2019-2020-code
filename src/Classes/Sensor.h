#ifndef SENSOR_H
#define SENSOR_H

//class for reading seensors
//derived from IO.h
#include "IO.h"
class Sensor: public IO{

public:
  Sensor(string name):IO(name){};

  //pure virtual function: http://www.tutorialspoint.com/cplusplus/cpp_polymorphism.htm
  //still not completely sure how this works
  virtual float read() = 0;
  virtual void set(float value, bool decouple=0) = 0;
  virtual void decouple() = 0;
  virtual void couple() = 0;
};

#endif
