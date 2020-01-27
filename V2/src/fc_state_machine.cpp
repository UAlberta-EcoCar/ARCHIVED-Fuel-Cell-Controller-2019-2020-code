#include "mbed.h"

#include <fc_state_machine.h>
#include <error_checker.h>

#include <pin_defs.h>

#ifdef ENABLE_FC_TEST
uint32_t state = FC_TEST;
#else
uint32_t state = FC_INIT;
#endif

uint32_t get_fc_state() {
    // read-only access to state variable
    return state;
}

// todo - other digital pin defs
DigitalIn start(START);

// todo - define which timers we need
Timer timer;

void fc_state_machine_thread() {
    while (true) {
        if (check_all_errors()) {
            state = FC_FAULT;
        }

        if (state == FC_TEST) {
            ThisThread::sleep_for(50); // Add test state code here, if desired.
        }

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
}
