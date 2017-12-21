#include <mbed.h>

#include "IO.h"

IO::IO(string name){
  this->set_name(name);
}

void IO::set_name(string name){
  mu.lock();
  this->__name = name;
  mu.unlock();
};

string IO::get_name(){
  return this->__name;
};

void IO::lock(){
  mu.lock();
};

void IO::unlock(){
  mu.unlock();
};
