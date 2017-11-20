#include <mbed.h>

#include "Integrator.h"

Integrator::Integrator(
  PinName pin,
  float error_threshold=2000000000.0,
  float int_error_threshold=2000000000.0,
  std::string name = "default",
  bool type=0,
  float a=1,
  float b=0
)
: AnalogIn_Ext(
    pin,
    error_threshold,
    name,
    type,
    a,
    b
  )
{
  this->int_error_threshold = int_error_threshold;
  this->int_started = 0;
  this->int_value = 0;
}

void Integrator::update()
{
  mu.lock();
  this->value = read();
  _scale();
  if (!(this->int_started)){
    this->dt.start();
    this->int_started = 1;
  }
  this->dt.stop();
  this->int_value = this->int_value + (this->value * this->dt.read());
  this->dt.reset();
  }
  mu.unlock();
}

void Integrator::return_int(
  float &ref,
  bool update=0
)
{
  if (update){
    update();
  }
  mu.lock();
  *ref = this->int_value;
  mu.unlock();
}

void Integrator::set_int_error_threshold(
  float error_threshold
)
{
  mu.lock();
  this->int_error_threshold = int_error_threshold;
  mu.unlock();
}

void Integrator::set(
  float value,
  float int_value
)
{
  mu.lock();
  this->value = value;
  this->int_value = int_value;
  mu.unlock();
}
