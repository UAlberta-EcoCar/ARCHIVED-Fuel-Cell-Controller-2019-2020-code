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

// Analog_Sensor Objects
extern Analog_Sensor capvolt;
extern Analog_Sensor fccurr;
extern Analog_Sensor fcvolt;
extern Analog_Sensor capcurr;
extern Analog_Sensor motorvolt;
extern Analog_Sensor motorcurr;
extern Analog_Sensor press1;
extern Analog_Sensor press2;
extern Analog_Sensor press3;
extern Analog_Sensor press4;
extern Analog_Sensor fctemp1;
extern Analog_Sensor fctemp2;
extern Analog_Sensor temp1;
extern Analog_Sensor temp2;
extern Analog_Sensor temp3;
extern Analog_Sensor temp4;
extern Analog_Sensor temp5;

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
