#ifndef DIGITALOUT_EXT_H
#define DIGITALOUT_EXT_H

#include "IO.h"
#include "Printable.h"
#include <string>

// Class is used to make DigitalOut thread safe
class DigitalOut_Ext: public IO{

int value;
DigitalOut pin;

public:
// Constructor
DigitalOut_Ext(PinName pin,string name);

// Method declaration
void write(bool value);
int read();
string toString();
string toStringInfo();
string toStringHeader();
string toJsonObject();
};
#endif
