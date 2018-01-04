#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H

#include <vector>
#include "IO.h"

template<class T>
class SerialPrinter: public IO {
private:
  Serial printer;

public:
  SerialPrinter(
    string name,
    PinName tx,
    PinName rx,
    int baud
  ):IO(name), printer(tx,rx,baud)
  {};

  void print(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter
  ){
    this->lock();
    for(int i = (*T_iter).begin(); i != (*T_iter).end(); i++){
      printer.printf("%s ",(*(*T_vec)[i]).toString());
    }
    printer.printf("\n");
    this->unlock();
  };

  void print_names(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter
  )
  {
    this->lock();
    for(int i = (*T_iter).begin(); i != (*T_iter).end(); i++){
      (*(*T_vec)[i]).lock();
      printer.printf("%s ",(*(*T_vec)[i]).get_name());
      (*(*T_vec)[i]).unlock();
    }
    printer.printf("\n");
    this->unlock();
  };
  void print_info(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter
  )
  {
    this->unlock();
    for(int i = (*T_iter).begin(); i != (*T_iter).end(); i++){
      (*(*T_vec)[i]).lock();
      printer.printf("%s ", (*(*T_vec)[i]).toStringInfo());
      (*(*T_vec)[i]).unlock();
    }
    printer.printf("\n");
    this->unlock();
  };

  void print_name(
    T* obj
  )
  {
    this->lock();
    (*obj).lock();
    printer.printf("%s ", (*obj).get_name());
    (*obj).unlock();
    this->unlock();
  };

  void print_info(
    T* obj
  )
  {
    this->lock();
    (*obj).lock();
    printer.printf("%f ", (float)(*obj).toStringInfo());
    (*obj).unlock();
    this->unlock();
  };
};
#endif
