#ifndef SERIALPRINTER_H
#define SERIALPRINTER_H

#include <vector>
#include "IO.h"

class SerialPrinter: public IO {
private:
  Serial printer;
  string stat;
public:

  SerialPrinter(
    string name,
    PinName tx,
    PinName rx,
    int baud
  ):IO(name), printer(tx,rx,baud){
    this->lock();
    this->stat = "";
    this->unlock();
  };

  template <class T>
  void print(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter,
    bool newline=1
  ){
    this->lock();
    stringstream ss;
    for((*T_iter) = (*T_vec).begin(); (*T_iter) != (*T_vec).end(); (*T_iter)++){
      (*(*(*T_iter))).lock();
      ss << (*(*(*T_iter))).toString() + " ";
      (*(*(*T_iter))).unlock();
    }
    if (newline){
      ss << "\n";
    }
    stat = ss.str();
    printer.printf("%s", this->stat.c_str());
    this->unlock();
  };

    template <class T>
  void print(
    T* obj,
    bool newline = 1
  )
  {
    this->lock();
    stringstream ss;
    (*obj).lock();
    ss << (*obj).toString() + " ";
    (*obj).unlock();
    if (newline){
      ss << "\n";
    }
    stat = ss.str();
    printer.printf("%s", this->stat.c_str());
    this->unlock();
  };

  template <class T>
  void print(
    T obj,
    bool newline = 1
  )
  {
    this->lock();
    stringstream ss;
    ss << obj;
    ss << " ";
    if (newline){
      ss << "\n";
    }
    stat = ss.str();
    printer.printf("%s", this->stat.c_str());
    this->unlock();
  };

  template <class T>
  void print_name(
    vector<T*>* T_vec,
    typename vector<T*>::iterator* T_iter,
    bool newline=1
  )
  {
    this->lock();
    stringstream ss;
    for((*T_iter) = (*T_vec).begin(); (*T_iter) != (*T_vec).end(); (*T_iter)++){
      (*(*(*T_iter))).lock();
      ss << (*(*(*T_iter))).get_name() + " ";
      (*(*(*T_iter))).unlock();
    }
    if (newline){
      ss << "\n";
    }
    stat = ss.str();
    printer.printf("%s", this->stat.c_str());
    this->unlock();
  };

  template <class T>
  void print_name(
    T* obj,
    bool newline = 1
  )
  {
    this->lock();
    stringstream ss;
    (*obj).lock();
    ss << (*obj).get_name();
    (*obj).unlock();
    if (newline){
      ss << "\n";
    }
    stat = ss.str();
    printer.printf("%s", this->stat.c_str());
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
    stringstream ss;
    for((*T_iter) = (*T_vec).begin(); (*T_iter) != (*T_vec).end(); (*T_iter)++){
      (*(*(*T_iter))).lock();
      ss << (*(*(*T_iter))).toStringInfo() + " ";
      (*(*(*T_iter))).unlock();
    }
    if (newline){
      ss << "\n";
    }
    stat = ss.str();
    printer.printf("%s", this->stat.c_str());
    this->unlock();
  };

  template <class T>
  void print_info(
    T* obj,
    bool newline = 1
  )
  {
    this->lock();
    stringstream ss;
    (*obj).lock();
    ss << (*obj).toStringInfo();
    (*obj).unlock();
    if (newline){
      ss << "\n";
    }
    stat = ss.str();
    printer.printf("%s", this->stat.c_str());
    this->unlock();
  };
};
#endif
