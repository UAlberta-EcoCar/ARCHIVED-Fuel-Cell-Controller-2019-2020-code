#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H
#include <vector>
#include "IO.h"

template<class T>
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

  void print(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter
  ){
    for(int i = (*T_iter).begin(); i != (*T_iter).end(); i++){
      printer.printf("%s %f ",(*(*T_vec)[i]).name(), (string)(*(*T_vec)[i]).read());
    }
  };

  void print_names(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter
  )
  {

  };

  void print_name(
    T* obj
  )
  {
    printer.printf("%s ", (*obj).get_name());
  };

  void print_read(
    T* obj
  )
  {
    printer.printf("%f ", (*obj).read());
  };
};
#endif
