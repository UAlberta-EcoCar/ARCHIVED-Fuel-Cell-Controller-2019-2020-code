#ifndef SENSOR_H
#define SENSOR_H

#include "IO.h"

class Sensor: public IO{

public:
  Sensor(string name):IO(name){};
  virtual float read(bool update=0) = 0;
  virtual void set(float value, bool decouple=1) = 0;
  virtual void update() = 0;
  virtual void decouple() = 0;
  virtual void couple() = 0;
};

#endif
