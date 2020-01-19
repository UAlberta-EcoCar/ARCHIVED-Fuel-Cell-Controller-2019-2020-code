#include "mbed.h"

#include <fc_state_machine.h>

#include <pin_defs.h>

uint32_t state = FC_INIT;

uint32_t get_fc_state() {
    // read-only access to state variable
    return state;
}

// todo - other digital pin defs
DigitalIn start(START);

// todo - define which timers we need
Timer timer;

void fc_state_machine_thread() {
    while (state != FC_FAULT) {

        if (state == FC_INIT){
            if (start) {
                state = FC_STARTUP;
                // we will need lots if timers
                // timer example:
                timer.start();
            }
        }
        else if(state == FC_STARTUP) {
            if (timer.read() > 5.0) {
                timer.stop();
                //do something here
            }
        }
        else if(state == FC_FAULT) {
            // Turn off all digital outputs here
        }
        else {
            // fault since invalid state entered
            state = FC_FAULT;
        }
        ThisThread::sleep_for(0.05);
    }
    // Turn off all digital outputs here in case fault state somehow missed them
}
