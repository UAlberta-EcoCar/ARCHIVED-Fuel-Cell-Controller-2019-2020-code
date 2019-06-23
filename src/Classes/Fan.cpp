#include "Fan.h"

using namespace std;

Fan::Fan(
  string name,
  PinName out_pin,
  PinName in_pin
)
:IO(name), out(out_pin)
{
  this->lock();
  this->out.period(0.00004);
  this->out.write(0.0);
  this->couple();
  this->unlock();
}

void Fan::set_out(
  float percentage
)
{
  this->lock();
  if (__coup){
     if (percentage <= 0){
      this->out.write(0.0);
    }
    else if (percentage >= 1){
      this->out.write(0.975);
    }
    else{
      this->out.write(percentage);
    }
  }
  this->unlock();
}

float Fan::get_out()
{
  return this->out.read();
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

string Fan::toString()
{
  stringstream ss;
  this->lock();
  ss << "Speed:";
  ss << this->get_out();
  this->unlock();
  return ss.str();
}

string Fan::toStringInfo()
{
  stringstream ss;
  this->lock();
  ss << this->get_out();
  this->unlock();
  return ss.str();
}

string Fan::toStringHeader()
{
  return this->get_name();
}

string Fan::toJsonObject()
{
  return "Not Implemented";
}

const float Fan::max = 0.95;
const float Fan::min = 0.40;
const float Fan::off = 0.0;
