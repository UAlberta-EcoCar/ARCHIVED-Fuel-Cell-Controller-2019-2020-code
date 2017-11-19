#include <mbed.h>
#include <rtos.h>

#include "pin_def.h"
/*
Define pins as AnalogIn -> reads an analog signal

AnalogIn name(pin);
*/
AnalogIn capvolt(CAPVOLT);
AnalogIn fccurr(FCCURR);
AnalogIn fcvolt(FCVOLT);
AnalogIn capcurr(CAPCURR);
//AnalogIn fcpres(FCPRES);
AnalogIn motorvolt(MOTORVOLT);
AnalogIn motorcurr(MOTORCURR);

AnalogIn press1(PRESS1);
AnalogIn press2(PRESS2);
AnalogIn press3(PRESS3);
AnalogIn press4(PRESS4);

AnalogIn fctemp1(FCTEMP1);
AnalogIn fctemp2(FCTEMP2);
AnalogIn temp1(TEMP1);
AnalogIn temp2(TEMP2);
AnalogIn temp3(TEMP3);
AnalogIn temp4(TEMP4);
AnalogIn temp5(TEMP5);

float capvolt_v;
float fccurr_v;
float fcvolt_v;
float capcurr_v;
float fcpres_v;

float fctemp1_v;
float fctemp2_v;
float temp1_v;
float temp2_v;
float temp3_v;
float temp4_v;
float temp5_v;

float press1_v;
float press2_v;
float press3_v;
float press4_v;

double fc_coulumbs_v;
double fc_joules_v;
double cap_coulumbs_v;
double cap_joules_v;

Mutex capvolt_m;
Mutex fccurr_m;
Mutex fcvolt_m;
Mutex capcurr_m;
Mutex fcpres_m;

Mutex fctemp1_m;
Mutex fctemp2_m;
Mutex temp1_m;
Mutex temp2_m;
Mutex temp3_m;
Mutex temp4_m;
Mutex temp5_m;

Mutex press1_m;
Mutex press2_m;
Mutex press3_m;
Mutex press4_m;

Mutex fc_coulumbs_m;
Mutex fc_joules_m;
Mutex cap_coulumbs_m;
Mutex cap_joules_m;

//TODO: Write getters and setters for new/remaining analog in's

// Getters
float get_capvolt(void){
  float value;
  capvolt_m.lock();
  value = capcurr_v;
  capvolt_m.unlock();

  return(value);
}

float get_capcurr(void){
  float value;
  capcurr_m.lock();
  value = capcurr_v;
  capcurr_m.unlock();

  return(value);
}

float get_fccurr(void){
  float value;
  fccurr_m.lock();
  value = fccurr_v;
  fccurr_m.unlock();

  return(value);
}

float get_fcvolt(void){return(fcvolt_v);}
// Will be replaced with getter(s) for press*
float get_fcpres(void){return(fcpres_v);}

double get_fc_coulumbs(void){return(fc_coulumbs_v);}
double get_fc_joules(void){return(fc_joules_v);}
double get_cap_coulumbs(void){return(cap_coulumbs_v);}
double get_cap_joules(void){return(cap_joules_v);}

// Setters

/*
Non-Integration setters:
  Calulates respective values, told by the function name,
  updating the value in the process.
  Args:
    void

  Returns:
    void
*/
void set_capvolt(void){
  capvolt_v = capvolt*45.768f+0.2715f;
}

void set_capcurr(void){
    capcurr_v = capcurr;
}

void set_fccurr(void){
  if(fccurr > 0.338){fccurr_v = 48.329f*fccurr-15.464f;}

  else{fccurr_v = 0.5f;}
}

void set_fcvolt(void){
  fcvolt_v = fcvolt*45.76f+0.2715f;
}

//void set_fcpres(void){
//  fcpres_v = fcpres*3.0f*9.93f-24.648f;
//}


/*
Integration setters:
  Calulates and Integrates the respective values, told by the function name,
  updating the value in the process.
  Args:
    dt (float)    Represents the change in time

  Returns:
    void
*/
void set_fc_coulumbs(float dt){
  fc_coulumbs_v += fccurr_v*dt;
}

void set_fc_joules(float dt){
  fc_joules_v += fccurr_v*fcvolt_v*dt;
}

void set_cap_coulumbs(float dt){
  cap_coulumbs_v += capcurr_v*dt;
}

void set_cap_joules(float dt){
  cap_joules_v += capcurr_v*capvolt_v*dt;
}
