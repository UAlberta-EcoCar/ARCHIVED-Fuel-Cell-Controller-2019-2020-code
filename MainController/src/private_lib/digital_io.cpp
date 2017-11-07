#include <mbed.h>
#include <rtos.h>
#include "pin_def.h"
#include "analog_io.h"
#include "fc_status.h"

//functions for accessing digital io functions from other locations
DigitalOut supply_v(SUPPLY_V);
DigitalOut purge_v(PURGE_V);
DigitalOut other1_v(VALVE3);
DigitalOut other2_v(VALVE4);

DigitalOut start_r(START_R);
DigitalOut motor_r(MOTOR_R);
DigitalOut charge_r(CHARGE_R);
DigitalOut cap_r(CAP_R);
DigitalOut fcc_r(FCC_R);

Mutex supply_m;
Mutex purge_m;
Mutex start_m;
Mutex motor_m;
Mutex charge_m;
Mutex cap_m;
Mutex fcc_m;

bool get_supply_valve(){
  bool value;
  supply_m.lock();
  value = supply_v;
  supply_m.unlock();

  return(value);
}

bool get_purge_valve(){
  bool value;
  purge_m.lock();
  value = purge_v;
  purge_m.unlock();

  return(value);
}

bool get_start_relay(){
  bool value;
  start_m.lock();
  value = start_r;
  start_m.unlock();

  return(value);
}

bool get_motor_relay(){
  bool value;
  motor_m.lock();
  value = motor_r;
  motor_m.unlock();
  return(value);
}

bool get_charge_relay(){
  bool value;
  charge_m.lock();
  value = charge_r;
  charge_m.unlock();
  return(value);
}

bool get_cap_relay(){
  bool value;
  cap_m.lock();
  value = cap_r;
  cap_m.unlock();

  return(value);
}
bool get_fcc_relay(){
  bool value;
  fcc_m.lock();
  value = fcc_r;
  fcc_m.unlock();
  return(value);}

void set_supply_valve(bool val){
  if(get_fc_status()!=ALARM_STATE){supply_v = val;}
  else{supply_v = 0;}
}

void set_purge_valve(bool val){
  if(get_fc_status()!=ALARM_STATE){purge_v = val;}
  else{purge_v = 0;}
}

void set_start_relay(bool val){
  if(get_fc_status()!=ALARM_STATE){
    if(charge_r == 0)
    {
      if(cap_r == 0){start_r = val;}
    }
  }
  else{start_r = 0;}
}

void set_motor_relay(bool val){
  if(get_fc_status()!=ALARM_STATE){motor_r = val;}
  else{motor_r = 0;}
}

void set_charge_relay(bool val)
{
  if(get_fc_status()!=0)
  {
    if(start_r == 0)
    {
      if(cap_r == 0){charge_r = val;}
    }
  }
  else{charge_r = 0;}
}

void set_cap_relay(bool val){
  if(get_fc_status()!=ALARM_STATE)
  {
    if(start_r == 0)
    {
      if(charge_r == 0)
      {
        if(get_capvolt()>19.0f){cap_r = val;}
      }
    }
  }
  else{cap_r = 0;}
}

void set_fcc_relay(bool val){fcc_r = val;}
