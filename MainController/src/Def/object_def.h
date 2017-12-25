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
extern Analog_Sensor<LinearScalable> capvolt;
extern Analog_Sensor<LinearScalable> fccurr;
extern Analog_Sensor<LinearScalable> fcvolt;
extern Analog_Sensor<LinearScalable> capcurr;
extern Analog_Sensor<LinearScalable> motorvolt;
extern Analog_Sensor<LinearScalable> motorcurr;
extern Analog_Sensor<LinearScalable> press1;
extern Analog_Sensor<LinearScalable> press2;
extern Analog_Sensor<LinearScalable> press3;
extern Analog_Sensor<LinearScalable> press4;
extern Analog_Sensor<LinearScalable> fctemp1;
extern Analog_Sensor<LinearScalable> fctemp2;
extern Analog_Sensor<LinearScalable> temp1;
extern Analog_Sensor<LinearScalable> temp2;
extern Analog_Sensor<LinearScalable> temp3;
extern Analog_Sensor<LinearScalable> temp4;
extern Analog_Sensor<LinearScalable> temp5;

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
