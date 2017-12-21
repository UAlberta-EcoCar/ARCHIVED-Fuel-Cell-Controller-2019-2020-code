#ifndef LINEARSCALABLE_H
#define LINEARSCALABLE_H

#include "Scalable.h"

class LinearScalable: public Scalable{
private:
  float a;
  float b;

public:
  LinearScalable();
  LinearScalable(float a);
  LinearScalable(float a, float b);
  float scale(float value) override;
  void set_params(float a=1, float b=0) override;
};

#endif
