#ifndef IO_H
#define IO_H

// Multi inheritance is bad, but whatever
// Private inheritance means the functions inherited can only be used internally
#include <string>
#include <mbed.h>

class IO{
private:
  string __name;
  Mutex mu;
public:
  IO(
    string name
  )
  {
  mu.lock();
  this->__name = name;
  mu.unlock();
  };

  void set_name(
    string name
  )
  {
    mu.lock();
    this->__name = name;
    mu.unlock();
  };
  string get_name()
  {
    return this->__name;
  };
  void lock(){
    mu.lock();
  };
  void unlock(){
    mu.unlock();
  };
};

#endif
