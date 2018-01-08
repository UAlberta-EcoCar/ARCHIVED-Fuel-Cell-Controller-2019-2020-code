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
        this->lock();
        if (this->_coup){
            (*this->master).lock();
            (*this->master).read(SHT31_DEFAULT_ADDR<<1,slave_buffer,6);
            (*this->master).unlock();

            uint16_t srh = slave_buffer[3];
            srh <<= 8;
            srh |= slave_buffer[4];
            srh *= 100;
            srh /= 0xFFFF;
            this->value = (float)srh;
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
  stringstream ss;
  ss << this->get_name();
  ss << ":";
  ss << fixed << setprecision(2) << this->read();
  return ss.str();
};

string HumiditySensor::toStringInfo()
{
  stringstream ss;
  ss << fixed << setprecision(2) << this->read();
  return ss.str();

}