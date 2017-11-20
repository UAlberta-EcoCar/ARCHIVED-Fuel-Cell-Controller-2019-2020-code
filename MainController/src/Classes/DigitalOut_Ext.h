#ifndef DIGITALOUT_EXT_H
#define DIGITALOUT_EXT_H

// Class is used to make DigitalOut thread safe
class DigitalOut_Ext: public DigitalOut{

int value;
Mutex mu;

public:
// Constructor
DigitalOut_Ext(PinName pin);

// Method declaration
void write_safe(int value);
int read_safe();

};
#endif
