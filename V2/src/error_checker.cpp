#include "mbed.h"

#include <error_checker.h>

#include <pin_defs.h>
#include <error_thresholds.h>
#include <analogs.h>
#include <fc_state_machine.h>

// Options I've considered for storing error state:
    // a map: slow
    // an array: fast. keeping track of indexes and array length is a pain
    // an uint: fast. keeping track of bits is a pain. Bitshifts are a pain
    // a struct: fast. Don't have to keep track of indexes. Can't iterate
uint32_t stateForError;
error_state_struct error_state;

error_state_struct get_error_state() {
    // provides read-only access to the error states
    return error_state;
}

bool check_all_errors() {
    // This is ugly but for now just manually list everything out here
    return error_state.fcvolt_high || error_state.fcvolt_low ||
           error_state.capvolt_high || error_state.capvolt_low ||
           error_state.fccurr_high || error_state.capcurr_high ||
           error_state.press_high || error_state.press_low||
           error_state.over_temp;
}

bool expect_low_voltage(uint32_t fc_state) {
    // It is expected that the voltage is low in initial and startup states
    return (fc_state==FC_STANDBY)||(fc_state==FC_SHUTDOWN);
}

bool expect_low_cap_voltage(uint32_t fc_state) {
    // It is expected that the capacitor is low when it is ok for the stack to be low
    // or the caps are charging
    return (fc_state==FC_STANDBY)||(fc_state==FC_CHARGE);
}

bool expect_low_pressure(uint32_t fc_state) {
    // Init and any state where the purge valve opens
    return (fc_state==FC_STANDBY)||(fc_state==FC_SHUTDOWN);
}

void error_checker_thread() {
    // Loop and check alert conditions at 10 Hz
    while (true) {
      stateForError = get_fc_state();
        if (get_analog_values().fcvolt > FCVOLT_MAX) {
            error_state.fcvolt_high = true;
        }
        if ((get_analog_values().fcvolt < FCVOLT_MIN) && (stateForError!=FC_STANDBY) && (stateForError!=FC_SHUTDOWN)) {
            error_state.fcvolt_low = true;
        }
        if (get_analog_values().capvolt > CAPVOLT_MAX) {
          error_state.capvolt_high = true;
        }
        if ((get_analog_values().capvolt < CAPVOLT_MIN) && (stateForError==FC_RUN)) {
          error_state.capvolt_low = true;
        }
        if (get_analog_values().fccurr > MAX_FC_CURR)  {
          error_state.fccurr_high = true;
        }
        if (get_analog_values().capcurr > MAX_CAP_CURR) {
          error_state.capcurr_high = true;
        }
        if (get_analog_values().press1 > PRESSURE_MAX) {
          error_state.press_high = true;
        }
        if ((get_analog_values().press1 < PRESSURE_MIN) && !expect_low_pressure(get_fc_state())) {
          error_state.press_low = true;
        }
        if ((get_analog_values().fctemp1 > TEMP_MAX)){
          error_state.over_temp = true;
        }
        if (get_relay_conflict()) {
          error_state.relays_shorted = true;
        }
        ThisThread::sleep_for(0.1);
   }
}
