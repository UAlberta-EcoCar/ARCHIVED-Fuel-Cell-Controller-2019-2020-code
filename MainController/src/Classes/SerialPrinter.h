#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H

#include <vector>
#include "IO.h"

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

  template <class T>
  void print(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter,
    bool newline=1
  ){
    this->lock();
    for((*T_iter) = (*T_vec).begin(); (*T_iter) != (*T_vec).end(); (*T_iter)++){
      (*(*(*T_iter))).lock();
      printer.printf("%s ",(*(*(*T_iter))).toString());
      (*(*(*T_iter))).unlock();
    }
    if (newline){
      printer.printf("\n");
    }
    this->unlock();
  };

  template <class T>
  void print_names(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter,
    bool newline=1
  )
  {
    this->lock();
    for((*T_iter) = (*T_vec).begin(); (*T_iter) != (*T_vec).end(); (*T_iter)++){
      (*(*(*T_iter))).lock();
      printer.printf("%s ",(*(*(*T_iter))).get_name());
      (*(*(*T_iter))).unlock();
    }
    
    if (newline){
      printer.printf("\n");
    }
    this->unlock();
  };

  template <class T>
  void print_info(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter,
    bool newline=1
  )
  {
    this->lock();
    for((*T_iter) = (*T_vec).begin(); (*T_iter) != (*T_vec).end(); (*T_iter)++){
      (*(*(*T_iter))).lock();
      printer.printf("%s ", (*(*(*T_iter))).toStringInfo());
      (*(*(*T_iter))).unlock();
    }

    if (newline){
      printer.printf("\n");
    }
    this->unlock();
  };

  template <class T>
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
  template <class T>
  void print_info(
    T* obj
  )
  {
    this->lock();
    (*obj).lock();
    printer.printf("%s ", (*obj).toStringInfo());
    (*obj).unlock();
    this->unlock();
  };
};
#endif
