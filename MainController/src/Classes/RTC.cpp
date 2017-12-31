#include "RTC.h"


RealTimeClock::RealTimeClock(
    string name,
    int address,
    I2C* master
):IO(name)
{
    this->lock();
    this->address = address;
    this->master = master;
    this->unlock();
};

string RealTimeClock::toString()
{
    this->__read();
    stringstream ss;
    ss << this->time.hour;
    ss << ":";
    ss << this->time.min;
    ss << ":";
    ss << this->time.sec;
    return ss.str();
};
string RealTimeClock::toStringInfo()
{
    return this->toString();
};

int RealTimeClock::second()
{
    this->__read();
    return this->time.sec;
};

int RealTimeClock::minute()
{
    this->__read();
    return this->time.min;
};

int RealTimeClock::hour()
{
    this->__read();
    return this->time.hour;
};

void RealTimeClock::__read()
{
    this->lock();
    (*master).lock();
    (*master).write(address<<1, 0, 1);
    Thread::wait(1);
    (*master).read(address<<1,this->slave_buffer, 19);
    (*master).unlock();

    this->time.sec = (slave_buffer[0]&0x0f) + ((slave_buffer[0]&0xf0)*10);
    this->time.min = (slave_buffer[1]&0x0f) + ((slave_buffer[1]&0xf0)*10);
    this->time.hour = (slave_buffer[2]&0x0f) + ((slave_buffer[2]&0xf0)*10);
    this->unlock();
};