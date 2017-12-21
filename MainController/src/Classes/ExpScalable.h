#ifndef EXPSCALABLE_H
#define EXPSCALABLE_H

#include "Scalable.h"

class ExpScalable: public Scalable{
private:
  float a;
  float b;

public:
  ExpScalable();
  ExpScalable(float a);
  ExpScalable(float a, float b);
  float scale(float value) override;
  void set_params(float a=1, float b=0) override;
};

#endif
