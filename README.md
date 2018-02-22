# Fuel Cell Controller Code
*Written for the 2017-2018 version of Fuel Cell Controller.*


The software for the Fuel Cell Controller is designed to make use of of mbed's RTOS functionality.


## Structure and task scheduling
The software is split into five-six different threads, each handling a different aspect of FCC. The overall scheduling scheme used is preemptive task scheduling. Therefore each thread has an assigned priority with the most real-time sensitive tasks (such as error handling) being placed in the higher priority threads.


The six threads are as follows, in order of highest to lowest priority:

* Error High
* Error Low
* Controller State Machine
* Monitoring
* Datalogging/Communications
* OLED Screen (*Not Yet Implemented*)

Each thread essentially is just an eventqueue, which dispatches forever, and a collection of events. Some events are posted to the queue periodically, and some posted after certain conditions are met.

### Monitoring and Controller State Machine


### Error's
Error's in the FCC are found by using the polling method. Every 50ms an `Event` for each error we want to monitor is posted to the lower error queue. Some error's use the interrupt method, such as the hydrogen alarm and emergency stop button's. This allows them to be delt with near instantly as opposed to having to wait to be polled, one of the main drawbacks of the polling method.

Each error check `Event` is a short method that preforms a few checks, posting an error `Event` to the higher error queue. These error `Events` can be specific although at the moment all error's use a single error handling task. This task kill's the statemachine thread and queue then set's the FCC into `ALARM_STATE`

#### Supported Error's
There are multiple error's that the FCC detects.
* FC Over-voltage
* FC Over-current
* Cap Over-voltage
* Cap Over-current
* Hydrogen Line Under-Pressure
* Hydrogen Line Over-Pressure
* FC Over-Temp
* FC Under-Temp
* H2 Sensor Alarm
* E-Stops 1 & 2

### Datalogging/Communications

### OLED Screen

## Features 

### Enabling/Disabling Features
There are a number of preproccer directives placed throughout the code to make the code as flexible as possible. Intended for diabling feature's if they don't yet work/hardware fails, draw significant resources, for testing purposes, or for specific vehicle configurations.

To enable/disable comment/uncomment the definitions found in the file `main.h`

#### Preprocessor Directives
##### Vehicle Specific
* ALICE_CONFIGURATION
    * changes the code so that it's suitible to be used in Alice, default configuation is Sofie
##### Additional, non-manditory I/O
* ENABLE_PRESS*
* ENABLE_TEMP*
* ENABLE_HUM
* ENABLE_AMBTEMP
* ENABLE_FAN*
* ENABLE_EXTERNAL_START
    * enables the use of the start button found on the steering wheel

##### On board chip enable
* ENABLE_RTC
* ENABLE_SHT31

##### Features/Tasks
* ENABLE_ERRORS
* ENABLE_DATALOGGING
* ENABLE_ANALYTICS

##### Datalogging Specifics
* ENABLE_BLUETOOTH
* ENABLE_OPENLOG
* ENABLE_OPENLOG_HEADER
* ENABLE_BLUETOOTH_SENSORS
* ENABLE_BLUETOOTH_INTG_VALUES
* ENABLE_BLUETOOTH_VANDR
* ENABLE_BLUETOOTH_FCSTATUS
* ENABLE_BLUETOOTH_FANS
* ENABLE_OPENLOG_SENSORS
* ENABLE_OPENLOG_INTG_VALUES
* ENABLE_OPENLOG_VANDR
* ENABLE_OPENLOG_FCSTATUS
* ENABLE_OPENLOG_FANS

##### Error's
* ENABLE_OVERVOLTAGE
* ENABLE_OVERCURR
* ENABLE_OVERPRESS
* ENABLE_UNDERPRESS
* ENABLE_OVERTEMP
* ENABLE_UNDERTEMP
* ENABLE_H2STOP
* ENABLE_ESTOP1
* ENABLE_ESTOP2

##### Testing
* ENABLE_TESTMODE
* ENABLE_RELAY_TEST

