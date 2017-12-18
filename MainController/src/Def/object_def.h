#ifndef OBJECT_DEF_H
#define OBJECT_DEF_H

// DigitalOut_Ext Objects
extern DigitalOut_Ext supply_v;
extern DigitalOut_Ext purge_v;
extern DigitalOut_Ext other1_v;
extern DigitalOut_Ext other2_v;
extern DigitalOut_Ext start_r;
extern DigitalOut_Ext motor_r;
extern DigitalOut_Ext charge_r;
extern DigitalOut_Ext cap_r;
extern DigitalOut_Ext fcc_r;
extern DigitalOut_Ext error_throw;
extern DigitalOut_Ext alarm_led;
extern DigitalOut_Ext debug_led;
extern DigitalOut_Ext shut_led;
extern DigitalOut_Ext run_led;
extern DigitalOut_Ext start_led;
extern DigitalOut_Ext ol_rst;
extern DigitalOut_Ext hum_rst;

// AnalogIn_Ext Objects
extern AnalogIn_Ext capvolt;
extern AnalogIn_Ext fccurr;
extern AnalogIn_Ext fcvolt;
extern AnalogIn_Ext capcurr;
extern AnalogIn_Ext motorvolt;
extern AnalogIn_Ext motorcurr;
extern AnalogIn_Ext press1;
extern AnalogIn_Ext press2;
extern AnalogIn_Ext press3;
extern AnalogIn_Ext press4;
extern AnalogIn_Ext fctemp1;
extern AnalogIn_Ext fctemp2;
extern AnalogIn_Ext temp1;
extern AnalogIn_Ext temp2;
extern AnalogIn_Ext temp3;
extern AnalogIn_Ext temp4;
extern AnalogIn_Ext temp5;

//Fans
extern Fan fan1;
extern Fan fan2;
extern Fan fan3;


// Intergrators
extern Integrator fc_coulumbs;
extern Integrator fc_joules;
extern Integrator cap_coulumbs;
extern Integrator cap_joules;

// Interrupts
extern InterruptIn h2;
extern InterruptIn err;

#endif
