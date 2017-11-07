#include <mbed.h>
#include <rtos.h>
#include <mbed_events.h>

#include "PwmIn.h"
#include "pin_def.h"
#include "mutex_def.h"

PwmIn tach1(TACH_1);
PwmIn tach2(TACH_2);
PwmIn tach3(TACH_3);

PwmOut pwm1(PWM_1);
PwmOut pwm2(PWM_2);
PwmOut pwm3(PWM_3);

DigitalOut fans_switch(FAN_SWITCH_ALL);

// Sets the speed of fan 1
// percentage is a float 1 >= percentage >= 0
void set_fan_1(float percentage){
  fan_1_m.lock();

  // Period of 40 us
  pwm1.period_us(40);
  pwm1.write(percentage);

  fan_1_m.unlock();
}

// Sets the speed of fan 2
// percentage is a float 1 >= percentage >= 0
void set_fan_2(float percentage){
  fan_2_m.lock();

  // Period of 40 us
  pwm2.period_us(40);
  pwm2.write(percentage);

  fan_2_m.unlock();
}

// Sets the speed of fan 3
// percentage is a float 1 >= percentage >= 0
void set_fan_3(float percentage){
  fan_3_m.lock();

  // Period of 40 us
  pwm3.period_us(40);
  pwm3.write(percentage);

  fan_3_m.unlock();
}

// Reads the speed of fan 1
float read_fan_1(){
  // Period of 40 us
  float duty = tach1.dutycycle();
  return duty;
}

// Reads the speed of fan 2
float read_fan_2(){
  float duty = tach2.dutycycle();
  return duty;
}

// Reads the speed of fan 3
float read_fan_3(){
  float duty = tach3.dutycycle();
  return duty;
}

// Function contols the power to all three fans
void state_fans(bool state){
  fan_switch_m.lock();

  fans_switch = state;

  fan_switch_m.unlock();
}

// Function controls the speed of all three fans
void set_fans(float percentage){
  set_fan_1(percentage);
  set_fan_2(percentage);
  set_fan_3(percentage);
}
