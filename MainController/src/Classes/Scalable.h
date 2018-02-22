#ifndef SCALABLE_H
#define SCALABLE_H

#include <math.h>
template <class T>
class Scalable{

public:
  virtual T scale(T value) = 0;
};

#endif
