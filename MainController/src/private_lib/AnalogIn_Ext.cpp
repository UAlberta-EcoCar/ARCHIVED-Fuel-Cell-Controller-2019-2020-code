#include <mbed.h>
#include <limits>
#include <math.h>

#include "AnalogIn_Ext.h"

using namespace std;

// Constructor method
AnalogIn_Ext::AnalogIn_Ext(
            PinName pin,
            bool int_flag,
            float error_threshold,
            float int_error_threshold,
            //std::string name,
            bool type,
            float a,
            float b
          )
          : in(pin)
          {
            this->int_flag = int_flag;
            //this->name = name;
            this->type = type;
            this->a = a;
            this->b = b;
          }

// Method Definition
void AnalogIn_Ext::update()
         {
           mu.lock();
           this->value = this->in.read();
           _scale();
           if (this->int_flag){
             if (!(this->int_started)){
               this->dt.start();
             }
             this->dt.stop();
             this->int_value = this->int_value + (this->value * this->dt.read());
             this->dt.reset();
           }
           mu.unlock();
         }

//TODO: Update and return methods, still need to figure out best way to do this

void AnalogIn_Ext::set_int_flag(bool int_flag)
        {
          mu.lock();
          this->int_flag = int_flag;
          mu.unlock();
        }

void AnalogIn_Ext::set_conv_param(float a, float b)
        {
          mu.lock();
          this->a = a;
          this->b = b;
          mu.unlock();
        }

void AnalogIn_Ext::set(float value, float int_value)
        {
          mu.lock();
          this->value = value;
          this->int_value = int_value;
          mu.unlock();
       }

void AnalogIn_Ext::_scale()
      {
        mu.lock();
        switch (this->type) {
          case 0: //Linear
            this->value = this->a * this->value + this->b;
          case 1: // Exponential, TODO: Calculate if this will even fit in a float at maximum in
            this->value = this->a * exp(this->value) + this->b;
        }
        mu.unlock();
      }
