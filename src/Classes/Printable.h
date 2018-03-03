#ifndef PRINTABLE_H
#define PRINTABLE_H
#include <string>

class Printable{

public:
    Printable(){};
    virtual string toString() = 0;
    virtual string toStringInfo() = 0;
    virtual string toStringHeader() = 0;

    virtual string toJsonObject() = 0;
};


#endif