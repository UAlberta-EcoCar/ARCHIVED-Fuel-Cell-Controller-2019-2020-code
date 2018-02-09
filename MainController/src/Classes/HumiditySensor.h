#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include "Sensor.h"
#include "SHT31.h"
using namespace std;

class HumiditySensor: public Sensor{
    private:
    bool _coup;
    int value;
    SHT31* sht31;

    public:
    HumiditySensor(string name, SHT31* sht31);
    float read(bool update=1);
    void set(float value, bool decouple=0);
    void update();
    void decouple();
    void couple();
    virtual string toString();
    virtual string toStringInfo();
};

#endif