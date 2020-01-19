#ifndef SCALABLE_H
#define SCALABLE_H

#include <math.h>

//Base class for applying scale factors to data
template <class T>
class Scalable{

public:
  //pure virtual function https://www.tutorialspoint.com/cplusplus/cpp_polymorphism.htm
  //still not completely sure how this works
  virtual T scale(T value) = 0;
};

#endif
