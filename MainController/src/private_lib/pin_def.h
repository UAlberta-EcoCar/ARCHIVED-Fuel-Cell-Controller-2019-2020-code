#ifndef PIN_DEF_H
#define PIN_DEF_H

//LED
#define STATUS_LED PTC5

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
// #define START_R not yet defined
#define MOTOR_R PB_3
#define START_R PB_2

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
// #define FCPRES PTC1 A11 is not defined for some reason will have to tie it to A19 which is near by

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
  -Ethernet #6
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
#define TACH_2 PC_6
#define TACH_3 PC_7
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
#define OL_CTS PE_3//CTS 2
#define OL_RTS PE_4//RTS 2
#define OL_TX PE_5//TX 2
#define OL_RX PE_6//RX 2

//FTDI Chip
#define FTDI_CTS PE_10//CTS 7
#define FTDI_RTS PE_9//RTS 7
#define FTDI_TX PE_8//TX 7
#define FTDI_RX PE_7//RX 7

//Bluetooth
#define BLUE_CTS PC_9//CTS 5
#define BLUE_RTS PC_8//RTS 5
#define BLUE_TX PC_12//TX 5
#define BLUE_RX PD_2//RX 5

//To motor controller
#define MOTOR_CTS PD_11//CTS 3
#define MOTOR_RTS PD_12//RTS 3
#define MOTOR_TX PB_10//TX 3
#define MOTOR_RX PB_11//RX 3

//Error Interrupt
#define ERROR_ISR PH_1
#define ERROR_ISR_THROW PH_0

/*
I2C
  -Used for real time clock and humidifier
*/
#define I2C_SDA PB_9
#define I2C_SCL PB_8

#endif
