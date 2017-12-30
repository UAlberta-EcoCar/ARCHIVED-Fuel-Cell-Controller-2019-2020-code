#ifndef EXPSCALABLE_H
#define EXPSCALABLE_H

#include "Scalable.h"
template <class T>
class ExpScalable: public Scalable{
private:
  T a;
  T b;

public:
  ExpScalable(){this->set_params();};
  ExpScalable(T a){this->set_params(a);};
  ExpScalable(T a, T b){this->set_params(a, b);};
  T scale(T value) {return (a*exp(value)) + b;};
  void set_params(T a=1, T b=0) {  
    this->a = a;
    this->b = b;
    };
};

#endif
