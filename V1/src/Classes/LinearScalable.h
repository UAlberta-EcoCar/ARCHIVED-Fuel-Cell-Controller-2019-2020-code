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
  //class constructors
  LinearScalable(){this->set_params();};
  LinearScalable(T a){this->set_params(a);}; //b defaults to 0
  LinearScalable(T a, T b){this->set_params(a, b);};

  //apply's linear scale
  T scale(T value){
    return (a*value) + b;
  };

  //changes scale factors
  void set_params(T a=1, T b=0){
    this->a = a;
    this->b = b;
  };
};

#endif
