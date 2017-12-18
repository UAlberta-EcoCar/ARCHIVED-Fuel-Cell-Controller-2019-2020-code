#ifndef INTEGRATOR_H
#define INTEGRATOR_H

// TODO: add way for more than one pin to integrate.

#include <string>
#include <vector>
#include "AnalogIn_Ext.h"

class Integrator{
  float int_value;
  float y;
  float int_error_threshold;
  vector<AnalogIn_Ext*> vec;
  vector<AnalogIn_Ext*>::iterator iter;

public:

//Constructor
Integrator(
            float int_error_threshold=2000000000.0,
            std::string name = "default"
          );

// Methods
void vec_add(AnalogIn_Ext* obj);
float return_int();
void set_int_error_threshold(float error_threshold);
// Overriden Methods
void update(float dt);
void set(float int_value);
};


#endif
