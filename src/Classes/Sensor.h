#ifndef SENSOR_H
#define SENSOR_H

#include "IO.h"

//base class for reading sensors
class Sensor: public IO{

public:
  Sensor(string name):IO(name){};

  //pure virtual function: https://www.tutorialspoint.com/cplusplus/cpp_polymorphism.htm
  //still not completely sure how this works
  virtual float read() = 0;
  virtual void set(float value, bool decouple=0) = 0;
  virtual void decouple() = 0;
  virtual void couple() = 0;
};

#endif
