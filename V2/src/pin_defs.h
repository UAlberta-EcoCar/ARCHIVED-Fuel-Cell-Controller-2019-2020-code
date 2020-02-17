#ifndef PIN_DEF_H
#define PIN_DEF_H

//OLED
#define OLDE_CLK PE_2
#define OLED_SEL PE_4
#define OLED_MISO PE_5
#define OLED_MOSI PE_6

//LED
#define ALARM_LED PG_0
#define DEBUG_LED PG_1
#define SHUT_LED PG_2
#define RUN_LED PG_9
#define START_LED PG_3

// External ADC
#define ADC_SCL PF_14
#define ADC_SDA PF_15

// Start
#define START PF_12
#define BUTTON PF_2
#define BLUE_BUTTON PC_13

/*
Valves
  -Ethernet #8
*/
#define SUPPLY_V PC_10
#define PURGE_V PC_11
#define VALVE3 PD_0
#define VALVE4 PD_1

/*
Relays
  -Ethernet #2
*/
#define CAP_R PB_2
#define CHARGE_R PB_4
#define FCC_R PB_0
#define MOTOR_R PB_3
#define START_R PC_7

/*
Analog voltage readings
  -Ethernet #3
*/
#define FCVOLT PA_0
#define MOTORVOLT PA_3
#define CAPVOLT PA_4
#define FCCURR PA_5
#define MOTORCURR PA_6
#define CAPCURR PA_7

/*
Analog voltage readings (pressure sensors)
  -Ethernet #1
*/
#define PRESS1 PB_1
#define PRESS2 PC_0
#define PRESS3 PC_2
#define PRESS4 PC_3

/*
H2 Board pins
  -Ethernet #5
*/
#define STOP PB_5
#define H2_OK PB_6
#define ESTOP1 PB_12
#define ESTOP2 PB_13

/*
Fans
  -Ethernet #7
*/
#define TACH_1 PB_15
#define TACH_2 PC_8
#define TACH_3 PC_9
#define PWM_1 PE_11
#define PWM_2 PE_12
#define PWM_3 PE_14
#define FAN_SWITCH_ALL PE_3

/*
Analog voltage readings (Temperture Sensors)
  -Ethernet #4
*/
#define FCTEMP1 PF_3
#define FCTEMP2 PF_4
#define TEMP1 PF_5
#define TEMP2 PF_7
#define TEMP3 PF_8
#define TEMP4 PF_9
#define TEMP5 PF_10


//Serial Defines
//open log
#define OPEN_LOG_TX PD_5//TX 2
#define OPEN_LOG_RX PD_6//RX 2
#define OPEN_LOG_RST PD_7

//FTDI Chip
#define FTDI_TX PE_8//TX 7
#define FTDI_RX PE_7//RX 7

//Bluetooth
#define BLUETOOTH_TX PC_12//TX 5
#define BLUETOOTH_RX PD_2//RX 5

//To motor controller
#define MOTOR_TX PB_10//TX 3
#define MOTOR_RX PB_11//RX 3

//Error Interrupt
// Not used
#define ERROR_ISR PH_1
#define ERROR_ISR_THROW PH_0

#define HUM_RST PC_6

/*
I2C
  -Used for real time clock and humidifier
*/
#define I2C_SDA PB_9
#define I2C_SCL PB_8

/*
Global Digital IO Objects
*/
extern DigitalIn start;
extern DigitalIn button;
extern DigitalIn blue_button;

extern DigitalOut start_r;
extern DigitalOut motor_r;
extern DigitalOut charge_r;
extern DigitalOut cap_r;
extern DigitalOut fcc_r;

extern DigitalOut supply_v;
extern DigitalOut purge_v;
extern DigitalOut other1_v;
extern DigitalOut other2_v;

#endif
