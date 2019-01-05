#ifndef LINEARSCALABLE_H
#define LINEARSCALABLE_H

#include "Scalable.h"

//Class for applying linear transform to data
//Derived from Scalable.h
template <class T>
class LinearScalable: public Scalable<T>{
private:
  T a;  //slope
  T b;  //intercept

public:
  LinearScalable(){this->set_params();};
  LinearScalable(T a){this->set_params(a);};
  LinearScalable(T a, T b){this->set_params(a, b);};

  T scale(T value){
    return (a*value) + b;
  };

  void set_params(T a=1, T b=0){
    this->a = a;
    this->b = b;
  };
};

#endif
