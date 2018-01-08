#ifndef INTEGRATOR_H
#define INTEGRATOR_H

//TODO: Add way to add constants to integration term and template the class

#include <vector>
#include "IO.h"

class Integrator: public IO{
private:
  float value;
  float y;
  vector<Sensor*> vec;
  vector<Sensor*>::iterator iter;
  Timer dt;
  bool _coup;

public:
//Constructor
Integrator(
  std::string name = "default"
):IO(name)
{
  this->lock();
  this->value = 0.0;
  this->_coup = 1;
  this->unlock();
};

// Methods
void sensor_add(
  Sensor* obj
)
{
  this->lock();
  this->vec.push_back(obj);
  this->unlock();
};

float read(
  bool update=1
)
{
  if (update){
    this->update();
  }
  return this->value;
};

void set(
  float value
)
{
  this->lock();
  this->value = value;
  this->unlock();
};

void update()
{
  this->lock();
  this->dt.stop();
  this->y=0.0;

  for (iter = vec.begin(); iter != vec.end(); iter++){
    this->y = this->y + (*(*iter)).read();
  }

  this->value = this->value + (((this->value + this->y)/2) * this->dt.read());
  this->dt.reset();
  this->unlock();
};

void start(){
  this->lock();
  this->dt.start();
  this->unlock();
};

void couple(
  bool couple=1
)
{
  this->lock();
  this->_coup = couple;
  this->unlock();
};

string toString()
{
  stringstream ss;
  ss << this->get_name();
  ss << ":";
  ss << fixed << setprecision(2) << this->read();
  return ss.str();
};

string toStringInfo()
{
  stringstream ss;
  ss << fixed << setprecision(2) << this->read();
  return ss.str();

};
};


#endif
