#include "SHT31.h"

SHT31::SHT31(string name, I2C *master):IO(name){
    this->lock();
    this->master = master;
    this->unlock();
}

void SHT31::read_device(){
    char data[SHT31_BUFFERSIZE] = {};
    char command[SHT31_CMDSIZE] = {(SHT31_MEAS_MEDREP_STRETCH&0xff00)>>8,
                                SHT31_MEAS_MEDREP_STRETCH&0xff};

    uint32_t hum = 0;
    uint32_t temp = 0;

    (*(this->master)).lock();
    (*(this->master)).write(SHT31_DEFAULT_ADDR<<1, command, SHT31_CMDSIZE);
    //Thread::wait(6);
    (*(this->master)).read(SHT31_DEFAULT_ADDR<<1, data, SHT31_BUFFERSIZE);
    (*(this->master)).unlock();

    // If null pointer exit
    if (data == 0){
        return;
    }

    // Ignore checksums for right now, don't really care toooooooo much
    temp = (data[0]<<8)|(data[1]);
    hum = (data[3]<<8)|(data[5]);

    // Temp(celcius) = -45 + 175*(raw sensor data / 2^16 - 1)
    temp = -45 + ((175*temp)/0xffff);
    // humidity(%) = 100*(raw sensor data / 2^16 - 1)
    hum = (100*hum)/0xffff;
    this->lock();
    this->readings.temp = temp;
    this->readings.humidity = hum;
    this->unlock();
}

int SHT31::get_temp(bool update){
    if (update){
        this->read_device();
    }
    return this->readings.temp;
}

int SHT31::get_hum(bool update){
    if (update){
        this->read_device();
    }
    return this->readings.humidity;
}

string SHT31::toString(bool json_format){
    this->read_device();
    stringstream ss;
    #ifdef ENABLE_AMBTEMP
    ss << "Amb Temp:";
    ss << this->get_temp();
    ss << " ";
    #endif

    #ifdef ENABLE_HUM
    ss << "Humidity:";
    ss << this->get_hum();
    #endif

    return ss.str();
}

string SHT31::toStringInfo(bool json_format){
    this->read_device();
    stringstream ss;
    #ifdef ENABLE_AMBTEMP
    ss << this->get_temp();
    ss << " ";
    #endif

    #ifdef ENABLE_HUM
    ss << this->get_hum();
    #endif

    return ss.str();
}