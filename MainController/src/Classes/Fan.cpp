#include "Fan.h"

using namespace std;

Fan::Fan(
  string name,
  PinName out_pin,
  PinName in_pin
)
:IO(name), out(out_pin), in(in_pin)
{
  this->lock();
  this->out.period(0.00004);
  this->out.write(0.0);
  this->__dutyIn = 0.0;
  this->unlock();
}

void Fan::set_out(
  float percentage
)
{
  this->lock();
  if (__coup){
     if (percentage < 0){
      this->out.write(0.0);
    }
    else if (percentage > 1){
      this->out.write(1.0);
    }
    else{
      this->out.write(percentage);
    }
  }
  this->unlock();
}

void Fan::set_in(
  float percentage,
  bool lock
)
{
  this->lock();

  if (percentage < 0){
      this->__dutyIn = 0.0;
  }
  else if (percentage > 1){
      this->__dutyIn = 1.0;
  }
  else{
      this->__dutyIn = percentage;
  }
  this->unlock();
}

float Fan::get_out()
{
  return this->out.read();
}

float Fan::get_in(){
  if(this->__coup){
    this->__dutyIn = this->in.dutycycle();
  }
  return __dutyIn;
}



bool Fan::is_spooled()
{
  if (this->get_in() >= this->get_out() - 0.1f){
    return true;
  }
  
  return false;
}

void Fan::couple()
{
  this->lock();
  this->__coup = 1;
  this->unlock();
}

void Fan::decouple()
{
  this->lock();
  this->__coup = 0;
  this->unlock();
}
