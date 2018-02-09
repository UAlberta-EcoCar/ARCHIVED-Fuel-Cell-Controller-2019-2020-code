#ifndef MAIN_H
#define MAIN_H

/*
 Place PreProccessor definitions here:
    A list of what they do can be found in the documentation.
        -Commenting them out disables them
*/
// Vehicle Specifier
// #define ALICE_CONFIGURATION

// Sensor enable/disable
#define ENABLE_CAPVOLT
#define ENABLE_CAPCURR
#define ENABLE_FCCURR
#define ENABLE_FCVOLT
#define ENABLE_MOTORVOLT
#define ENABLE_MOTORCURR
#define ENABLE_PRESS1
// #define ENABLE_PRESS2
// #define ENABLE_PRESS3
// #define ENABLE_PRESS4 
#define ENABLE_FCTEMP1
#define ENABLE_FCTEMP2
// #define ENABLE_TEMP1
// #define ENABLE_TEMP2
// #define ENABLE_TEMP3
// #define ENABLE_TEMP4
// #define ENABLE_TEMP5
#define ENABLE_HUM
#define ENABLE_AMBTEMP

// Fan enable/disable
#define ENABLE_FAN1
#define ENABLE_FAN2
#define ENABLE_FAN3

// TODO: Add into code
// Digital Out enable/disable
#define ENABLE_SUPPLYV
#define ENABLE_PURGEV
#define ENABLE_STARTR
#define ENABLE_MOTORR
#define ENABLE_CHARGER
#define ENABLE_CAPR
#define ENABLE_FCCR
#define ENABLE_ALED
#define ENABLE_DLED
#define ENABLE_SLED
#define ENABLE_RLED
#define ENABLE_SHLED

// In code
// Define feature enable/disable
// #define ENABLE_ERRORS
#define ENABLE_DATALOGGING
// not in code
#define ENABLE_ANALYTICS

//In code
// Datalogging Specifics enable/disable
#define ENABLE_BLUETOOTH
#define ENABLE_OPENLOG
#define ENABLE_OPENLOG_HEADER
#define ENABLE_BLUETOOTH_SENSORS
#define ENABLE_BLUETOOTH_INTG_VALUES
#define ENABLE_BLUETOOTH_VANDR
#define ENABLE_BLUETOOTH_FCSTATUS
#define ENABLE_BLUETOOTH_FANS
#define ENABLE_OPENLOG_SENSORS
#define ENABLE_OPENLOG_INTG_VALUES
#define ENABLE_OPENLOG_VANDR
#define ENABLE_OPENLOG_FCSTATUS
#define ENABLE_OPENLOG_FANS

// In code
// Error's enable/disable
#define ENABLE_OVERVOLTAGE
#define ENABLE_OVERCURR
#define ENABLE_OVERPRESS
#define ENABLE_UNDERPRESS
#define ENABLE_OVERTEMP
#define ENABLE_UNDERTEMP
#define ENABLE_H2STOP
#define ENABLE_ESTOP1
#define ENABLE_ESTOP2

#endif
