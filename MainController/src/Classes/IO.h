#ifndef IO_H
#define IO_H

// Multi inheritance is bad, but whatever
// Private inheritance means the functions inherited can only be used internally
#include <string>

class IO{
private:
  string __name;
  Mutex mu;
public:
  IO(string name);
  void set_name(string name);
  string get_name();
  void lock();
  void unlock();
};

#endif
