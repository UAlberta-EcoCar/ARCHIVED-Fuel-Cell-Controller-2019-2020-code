#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include "Sensor.h"
#include "./Def/SHT31.h"
using namespace std;

class HumiditySensor: public Sensor{
    private:
    bool _coup;
    float value;
    char slave_buffer[6];

    I2C* master;

    public:
    HumiditySensor(string name, I2C* master);
    float read(bool update=1);
    void set(float value, bool decouple=1);
    void update();
    void decouple();
    void couple();
    string toString();
    string toStringInfo();
};

#endif