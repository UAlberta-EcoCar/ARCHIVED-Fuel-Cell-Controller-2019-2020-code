#include <mbed.h>

#include "Fan.h"

using namespace std;

Fan::Fan(
  PinName out_pin,
  PinName in_pin
)
:PwmOut(out_pin), PwmIn(in_pin)
{
  mu.lock();
  this->period(0.00004);
  this->write(0.0);
  this->__dutyIn = 0.0;
  mu.unlock();
}

void Fan::set_dutycycle(
  float percentage
)
{
  mu.lock();
  if (percentage < 0){
      this->write(0.0);
  }
  else if (percentage > 1){
      this->write(1.0);
  }
  else{
      this->write(percentage);
  }
  mu.unlock();
}

void Fan::set_in_dutycycle(
  float percentage,
  bool lock
)
{
  mu.lock();

  if (lock){
    this->lock();
  }

  if (percentage < 0){
      this->__dutyIn = 0.0;
  }
  else if (percentage > 1){
      this->__dutyIn = 1.0;
  }
  else{
      this->__dutyIn = percentage;
  }
  mu.unlock();
}

float Fan::get_dutycycle()
{
  return this->read();
}

float Fan::get_in_dutycycle(){
  if(!this->__lock){
    this->__dutyIn = this->dutycycle_in();
  }
  return __dutyIn;
}

void Fan::unlock()
{
  mu.lock();
  this->__lock = 1;
  mu.unlock();
}

bool Fan::is_spooled()
{
  if (this->get_in_dutycycle() >= this->get_dutycycle() - 0.1f){
    return true;
  }
  else{
    return false;
  }
}

void Fan::lock()
{
  mu.lock();
  this->__lock = 0;
  mu.unlock();
}
