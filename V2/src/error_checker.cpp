#include "mbed.h"

#include <error_checker.h>

#include <error_thresholds.h>
#include <analogs.h>
#include <fc_state_machine.h>

error_state_struct error_state;

error_state_struct get_error_state() {
    // provides readonly access to the error states
    return error_state;
}

bool expect_low_voltage(uint32_t fc_state) {
    // It is expected that the voltage is low in initial and startup states
    return (fc_state==FC_INIT)|(fc_state==FC_STARTUP);
}

void error_checker_thread() {
    // Loop and check alert conditions at 10 Hz
    while (true) {
        if (get_analog_values().fcvolt > FCVOLT_MAX) {
            error_state.fcvolt_high = true;
        }
        if ((get_analog_values().fcvolt < FCVOLT_MIN)&!expect_low_voltage(get_fc_state())) {
            error_state.fcvolt_low = true;
        }
        
        ThisThread::sleep_for(0.1);
    }
}