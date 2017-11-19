#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

void state_fans(bool state);
void set_fans(float percentage);
float get_fans();

void set_fan_1(float percentage);
void set_fan_2(float percentage);
void set_fan_3(float percentage);

float get_fan_1();
float get_fan_2();
float get_fan_3();

#endif
