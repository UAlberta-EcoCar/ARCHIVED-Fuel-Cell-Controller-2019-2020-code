#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <string>

class Integrator: public AnalogIn_Ext{
  float int_value;
  float int_error_threshold;
  bool int_started;

  Timer dt;

public:

//Constructor
Integrator(
            PinName pin,
            float error_threshold=2000000000.0,
            float int_error_threshold=2000000000.0,
            std::string name = "default",
            bool type=0,
            float a=1,
            float b=0
          );

// Methods
void return_int(float &ref, bool update=0);
void set_error_threshold(float error_threshold);
// Overriden Methods
void update();
void set(float value, float int_value);
};


#endif
