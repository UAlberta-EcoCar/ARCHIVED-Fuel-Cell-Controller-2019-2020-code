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

### Datalogging
Datalogging consists of three periodically posted tasks, each task queries global I/O objects and then writes the data to it's respective serial port.

Since we have three serial ports being used (OpenLog, Bluetooth, FTDI) there are three tasks. At the moment bluetooth and the FTDI chip share the same data formatting. Openlog log prints a header on start up detailing column names, follwed by only the respective values in the following records. FTDI and the bluetooth follow the "name:value" format at this moment.

### Monitoring and the State Machine
These two threads together make up the normal control portion of the FCC. The combination of the two threads is essentially a thread safe state machine, with a few auxillary functions.

The state machine can be split into three parts.
* The State
* The Transisition
* The Transisition Control

#### The state and the transition
The state is the state of the DigitalOut pin. Each state has an `Event` dedicated for the transistion into said state. A state is persistent until one of two conditions happen.

<dl>
    <dt><b>Transition control posts a transistion.</b></dt>
    <dd>This happens when the transition controller determines that a transition is nessecary</dd>
    <dt><b>Error has been detected and state machine is shut-down</b></dt>
    <dd>When an error happens it shuts down the state-machine and then externally enters the alarm state.</dd>
</dl>

The transition code for each state is very simple, consisting of merely a few DigitalOut pin writes and a few bitwise operations. The format for transition events is:
```C++
Event<void()> transition_event(&queue, transition);

void transition(){
    // Header
    
    // Transition

    // Trailer
}
```
The header is responsible for clearing/setting up flags and setting the the fuel-cell state group (if transistioning between state groups). The transition is self explanitory. The trailer is responsible for indicating to the transistion controller the transition is complete. This is done with an `Event`.

> State Groups & States
> * Start 
>   * Start Entry
>   * Start Purge
>   * FC Charge Entry
>   * FC Charge Exit
> * Charge
>   * Charge Entry
>   * Cap Charge Start
>   * Cap Charge Exit
> * Run
>   * Run Entry
> * Purge
>   * Purge Entry
>   * Purge Exit
> * Shutdown
>   * Shutdown Entry
> * Alarm*
>
>*Alarm is not actually a part of the state machine, will be explained later*

State groups are a collection of multiple smaller sub-states. This is because a state-machine is static, any transformation of the state is a transition. The states required to achevieve an overall task (such as start-up) are grouped together. 

#### Transition Control

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
