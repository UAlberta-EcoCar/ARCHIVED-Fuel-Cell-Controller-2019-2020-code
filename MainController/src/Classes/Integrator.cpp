#include <mbed.h>

#include "Integrator.h"

//NOTE: SUPER FUCKING BROKEN RN, DO NOT TRY COMPILING

Integrator::Integrator(
  float int_error_threshold,
  std::string name
)
{
  this->int_error_threshold = int_error_threshold;
  this->int_value = 0;
}

void Integrator::vec_add(
  AnalogIn_Ext* obj
)
{
  this->vec.push_back(obj);
}

void Integrator::update(float dt)
{
  this->y=0.0;

  for (iter = vec.begin(); iter != vec.end(); iter++){
    this->y = this->y + (*(*iter)).return_val();
  }

  this->int_value = this->int_value + (this->y * dt);
}

float Integrator::return_int()
{
  return this->int_value;
}

void Integrator::set_int_error_threshold(
  float error_threshold
)
{
  this->int_error_threshold = int_error_threshold;
}

void Integrator::set(
  float int_value
)
{
  this->int_value = int_value;
}
