#include "HumiditySensor.h"

HumiditySensor::HumiditySensor(
    string name, 
    SHT31* sht31
    ):Sensor(name)
    {
        this->lock();
        this->sht31 = sht31;
        this->value = 0;
        this->couple();
        this->unlock();
    };

float HumiditySensor::read(bool update)
    {
        if (update){
            this->update();
        }

        return 1.0;
    };

void HumiditySensor::set(float value, bool decouple)
    {
        if (decouple){
            this->decouple();
        }

        this->lock();
        this->value = (int)value;
        this->unlock();
    };

void HumiditySensor::update()
    {
        this->lock();
        char slave_buffer[BUFFERSIZE];
        if (this->_coup){
            (*this->sht31).lock();
            (*this->sht31).read_slave(slave_buffer);
            (*this->sht31).unlock();

            uint16_t srh = slave_buffer[3];
            srh <<= 8;
            srh |= slave_buffer[4];
            srh *= 100;
            srh /= 0xFFFF;
            
            this->value = srh;
        }
        this->unlock();
    };

void HumiditySensor::decouple()
    {
        this->lock();
        this->_coup = 0;
        this->unlock();
    };
void HumiditySensor::couple()
    {
        this->lock();
        this->_coup = 1;
        this->unlock();
    };

string HumiditySensor::toString()
    {
        this->update();
        stringstream ss;
        ss << this->get_name();
        ss << ":";
        ss << this->value;
        return ss.str();
    };

string HumiditySensor::toStringInfo()
    {
        this->update();
        stringstream ss;
        ss << this->value;
        return ss.str();
    };