#ifndef POLYSCALABLE_H
#define POLYSCALABLE_H

#include "Scalable.h"
#include <vector>

template <class T, class V>
class PolyScalable: public Scalable{
private:
  vector<T> param_vec;
  typename vector<T>::iterator param_iter;

public:
  PolyScalable(
  )
  {
  };
  PolyScalable(
    T a
  )
  {
    this->param_vec.push_back(a);
    this->param_vec.push_back(0.0);
  };
  PolyScalable(
    T a,
    T b
  )
  {
    this->param_vec.push_back(a);
    this->param_vec.push_back(b);
  };
  PolyScalable(
    vector<T> params
  )
  {
    this->param_vec = params;
  };
  /*
  PolyScalable(
    T a, T b, ...
  )
  {
    va_list arguments;
    va_start(arguments, b);
    for (int i = 0)
  }
  */

  V scale(
    T value
  )
  {
    int count = 0;
    T val = 0;
    for (this->param_iter = this->param_vec.begin(); this->param_iter != this->param_vec.end(); this->param_iter++){
      val += (*this->param_iter)*pow(value, count);
      count++;
    }
    return (V) val;
  };

  void set_params(
  )
  {
  };

  void set_params(
    vector<T> params
  )
  {
    this->param_vec = params;
  };
};

#endif