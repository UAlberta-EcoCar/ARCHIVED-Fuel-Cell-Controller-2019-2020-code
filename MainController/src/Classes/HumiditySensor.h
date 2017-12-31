#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include "IO.h"
#include "Sensor.h"

class HumiditySensor: public Sensor{
    private:
    bool _coup;
    float value;

    I2C* master;

    public:
    HumiditySensor(string name, I2C* master);
    float read(bool update=0);
    void set(float value, bool decouple=1);
    void update();
    void decouple();
    void couple();
};

#endif