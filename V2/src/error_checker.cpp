#include "mbed.h"

#include <error_checker.h>

#include <error_thresholds.h>
#include <analogs.h>
#include <fc_state_machine.h>

// Options I've considered for storing error state:
    // a map: slow
    // an array: fast. keeping track of indexes and array length is a pain
    // an uint: fast. keeping track of bits is a pain. Bitshifts are a pain
    // a struct: fast. Don't have to keep track of indexes. Can't iterate
error_state_struct error_state;

error_state_struct get_error_state() {
    // provides read-only access to the error states
    return error_state;
}

bool check_all_errors() {
    // This is ugly but for now just manually list everything out here
    return error_state.fcvolt_high | error_state.fcvolt_low;
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