#ifndef FANCONTOL_H
#define FANCONTOL_H

#include <mbed.h>
#include <vector>
#include "Fan.h"
#include "Analog_Sensor.h"
#include "LinearScalable.h"
#include "PolyScalable.h"
#include "DigitalOut_Ext.h"

template <class T, class V>
class FanControl{

    private:

        // Fields
        static const float p;
        static const float i;
        static const float d;
        float out;
        float iterm;
        float prev_temp_diff;
        vector<Fan*> *fan_vec;
        typename vector<Fan*>::iterator fan_iter;
        vector<Analog_Sensor<T>* > *temp_sensor_vec;
        typename vector<Analog_Sensor<T>* >::iterator temp_sensor_iter;
        Analog_Sensor<V> *fccurr;
        DigitalOut_Ext *enable_pin;
        Timer dt;
        Mutex mu;

        // Methods
        void _set_fans(float value){
            for (fan_iter = (*fan_vec).begin(); fan_iter != (*fan_vec).end(); fan_iter++){
                (*(*fan_iter)).set_out(value);
            }
        };

        float _average_temp(){
            int count = 0;
            float ave = 0;

            for (temp_sensor_iter = (*temp_sensor_vec).begin(); temp_sensor_iter != (*temp_sensor_vec).end(); temp_sensor_iter++){
                ave += (*(*temp_sensor_iter)).read();
                count++;
            }
            if (count == 0){
                return 0;
            }
            return (ave/count);
        };

    public:

        // Constructors
        FanControl(vector<Fan*> *fan_vec_ptr,
                   vector<Analog_Sensor<T>* > *temp_vec_ptr, 
                   Analog_Sensor<V>* current, 
                   DigitalOut_Ext *power_pin=0){
            this->lock();
            this->fan_vec = fan_vec_ptr;
            this->temp_sensor_vec = temp_vec_ptr;
            this->fccurr = current;
            this->enable_pin = power_pin;
            this->out = 0;
            this->iterm = 0;
            this->prev_temp_diff = 0;
            this->unlock();
        };

        // Methods:
        // "Setters"
        void lock(){this->mu.lock();};
        void unlock(){this->mu.unlock();};

        void set(float value){
            this->lock();
            this->_set_fans(value);
            this->unlock();
        }

        void power(bool status = 0, DigitalOut_Ext *power_pin = 0){
            if (power_pin != 0){
                this->enable_pin = power_pin;
            }

            this->lock();

            if (this->enable_pin == 0){
                this->unlock();
                return;
            }

            (*this->enable_pin).write(status);
            this->unlock();
        };

        // Functional Methods
        void pid_start(){
            this->lock();
            this->dt.start();
            this->dt.reset();
            this->iterm = 0;
            this->prev_temp_diff = this->_average_temp();
            this->unlock();
        };

        void pid_update(){
            this->lock();
            // Get time passed
            float dt = this->dt.read();
            float current = (*(this->fccurr)).read();
            this->dt.reset();

            if (dt == 0.0 || dt >= 4.0){
                this->pid_start();
            }

            // Find difference/error
            float temp_diff = this->_average_temp() - FuelCell::query_optimal_temp(current);

            // Proportional term
            float porp_term = this->p * temp_diff;

            // Itergral Term
            this->iterm = (this->i * temp_diff * dt) + this->iterm;

            // Derivative Term
            float der_term = (this->d * (temp_diff - this->prev_temp_diff))/dt;

            // Sum then limit
            this->out = porp_term + der_term + this->iterm;
            if (this->out > Fan::max){
                this->out = Fan::max;
            }
            else if (this->out < Fan::min){
                this->out = Fan::min;
            }
            
            this->_set_fans(this->out);
            this->prev_temp_diff = temp_diff;
            this->unlock();
        };

};

template <class T, class V>
const float FanControl<T,V>::p = 1.0;

template <class T, class V>
const float FanControl<T,V>::i = 1.0;

template <class T, class V>
const float FanControl<T,V>::d = 1.0;

#endif