#ifndef SCALABLE_H
#define SCALABLE_H

#include <math.h>

class Scalable{

public:
  virtual float scale(float value) = 0;
  virtual void set_params(float a=1, float b=0) = 0;
};

#endif
