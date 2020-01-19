#ifndef PRINTABLE_H
#define PRINTABLE_H
#include <string>

class Printable{

public:
    Printable(){}; //Empty default constructor
    // Change to Printer struct, containing the header and info
	// All are pure virtual functions which forces "IO" to override them.
    virtual string toString() = 0;
    virtual string toStringInfo() = 0;
    virtual string toStringHeader() = 0;
    virtual string toJsonObject() = 0;
};


#endif