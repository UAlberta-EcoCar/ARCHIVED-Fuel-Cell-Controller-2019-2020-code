#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H
#include <vector>
#include "IO.h"

class SerialPrinter {
private:
  Serial printer;
  Mutex mu;

public:
  SerialPrinter(
    PinName tx,
    PinName rx,
    int baud
  ):printer(tx,rx,baud)
  {};

  print_names(
    vector<IO*>* IO_device_list,
    vector<IO*>::iterator* IO_device_list_iter
  ){

  };
  print_values(
    vector<IO*>* IO_device_list,
    vector<IO*>::iterator* IO_device_list_iter
  )
  {

  };

  print_name_value(
    vector<IO*>* IO_device_list,
    vector<IO*>::iterator* IO_device_list_iter
  )
  {

  };

};
#endif
