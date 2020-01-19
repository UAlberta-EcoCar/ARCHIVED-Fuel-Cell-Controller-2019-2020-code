#include "RTC.h"

RealTimeClock::RealTimeClock(string name, I2C *master):IO(name){
    this->lock();
    this->master = master;
    this->unlock();
}

int RealTimeClock::get_sec(bool update){
    return this->time.sec;
}

int RealTimeClock::get_min(bool update){
    return this->time.min;
}

int RealTimeClock::get_hour(bool update){
    return this->time.hour;
}

void RealTimeClock::read_device(){
    char data[DS3231_BUFFERSIZE] = {};

    // Read a byte from the seconds address of the DS3231, stores byte in data
    (*(this->master)).lock();
    (*(this->master)).read(DS3231_ADDRESS<<1, data, DS3231_BUFFERSIZE);
    (*(this->master)).unlock();

    // If null pointer exit
    if (data == 0){
        return;
    }

    // Parse Bytes
    int seconds = ((data[SECONDS])&0b1111) + (((data[SECONDS])&0b1110000)>>4)*10;
    int minutes = ((data[MINUTES])&0b1111) + (((data[MINUTES])&0b1110000)>>4)*10; 
    int hours = ((data[HOURS])&0b1111) + (((data[HOURS])&0b10000)>>4)*10; 

    // Store seconds in the sec slot of time struct
    this->lock();
    this->time.sec = seconds;
    this->time.min = minutes;
    this->time.hour = hours;
    this->unlock();
}

// TODO: Add json formats
string RealTimeClock::toString(bool json_format){
    stringstream ss;
    this->read_device();
    this->lock();
    ss << this->get_hour();
    ss << ":";
    ss << this->get_min();
    ss << ":";
    ss << this->get_sec();
    this->unlock();
    return ss.str();
}

string RealTimeClock::toStringInfo(bool json_format){
    return this->toString();
}
