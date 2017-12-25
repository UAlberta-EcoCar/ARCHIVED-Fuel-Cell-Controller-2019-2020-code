#ifndef LINEARSCALABLE_H
#define LINEARSCALABLE_H

#include <math.h>

#include "Scalable.h"

class LinearScalable: public Scalable{
private:
  float a;
  float b;

public:
  LinearScalable(
  )
  {
    this->set_params();
  };
  LinearScalable(
    float a
  )
  {
    this->set_params(a);
  };
  LinearScalable(
    float a,
    float b
  )
  {
    this->set_params(a, b);
  };

  float scale(
    float value
  )
  {
    return (a*value) + b;
  };

  void set_params(
    float a=1,
    float b=0
  )
  {
    this->a = a;
    this->b = b;
  };
};

#endif
