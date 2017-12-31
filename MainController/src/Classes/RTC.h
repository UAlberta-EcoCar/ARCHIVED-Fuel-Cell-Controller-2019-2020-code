#ifndef RTC_H
#define RTC_H

#include "IO.h"

class RealTimeClock: public IO{
// Class is specific to the DS3231 Real Time Clock
//      -Communicates via I2C
private:
int address;
struct{
    int hour;
    int min;
    int sec;
}time;
char slave_buffer[19];
I2C* master;

void __read();

public:

RealTimeClock(
    string name,
    int address,
    I2C* master
);

string toString();
string toStringInfo();
int second();
int minute();
int hour();

};

#endif