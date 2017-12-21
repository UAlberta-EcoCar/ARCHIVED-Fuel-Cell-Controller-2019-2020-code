#ifndef ANALOGIN_SENSOR_H
#define ANALOGIN_SENSOR_H

#include <string>
#include <mbed.h>
#include "Sensor.h"
#include "Scalable.h"
#include "LinearScalable.h"

class Analog_Sensor: public Sensor{
private:
  float value;
  bool _coup;
  Scalable* scaler;
  AnalogIn pin;

/*
Method declaration
*/
public:
// Constructor
Analog_Sensor(
  PinName pin,
  Scalable* scaler,
  string name="default"
);

void update() override;
float read(bool update=0) override;
void set(float value, bool decouple=1) override;
void couple() override;
void decouple() override;


};

#endif
