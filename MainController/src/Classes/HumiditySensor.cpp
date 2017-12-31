#include "HumiditySensor.h"
    
    
HumiditySensor::HumiditySensor(
    string name, 
    I2C* master
    ):Sensor(name)
    {
        this->lock();
        this->master = master;
        this->value = 0.0;
        this->unlock();
    };

float HumiditySensor::read(bool update)
    {
        if (update){
            this->update();
        }

        return this->value;
    };

void HumiditySensor::set(float value, bool decouple)
    {
    if (decouple){
        this->decouple();
    }
    this->lock();
    this->value = value;
    this->unlock();
    };

void HumiditySensor::update()
    {
        //TODO: Read the humidity sensor data sheet
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