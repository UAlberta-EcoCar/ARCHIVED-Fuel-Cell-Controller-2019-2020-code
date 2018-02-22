#include "DigitalOut_Ext.h"
#include <sstream>

using namespace std;

DigitalOut_Ext::DigitalOut_Ext(
  PinName pin,
  string name
):IO(name), pin(pin){};

// Method declaration
void DigitalOut_Ext::write(
  bool value
)
{
  this->lock();
  this->value = value;
  this->pin.write(value);
  this->unlock();
};

int DigitalOut_Ext::read(){
  return this->value;
};

string DigitalOut_Ext::toString()
{
  stringstream ss;
  ss << this->get_name();
  ss << ":";
  ss << this->read();
  return ss.str();
};

string DigitalOut_Ext::toStringInfo()
{
  stringstream ss;
  ss << this->read();
  return ss.str();
};

string DigitalOut_Ext::toStringHeader()
{
  return this->get_name();
};

string DigitalOut_Ext::toJsonObject()
{
  return "Not Implemented";
};