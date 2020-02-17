#include "mbed.h"

#include <error_checker.h>

#include <pin_defs.h>
#include <error_thresholds.h>
#include <analogs.h>
#include <fc_state_machine.h>

DigitalIn stop(STOP);
DigitalIn h2_ok(h2_ok);
DigitalIn estop1(ESTOP1);
DigitalIn estop2(ESTOP2);

// Options I've considered for storing error state:
// a map: slow
// an array: fast. keeping track of indexes and array length is a pain
// an uint: fast. keeping track of bits is a pain. Bitshifts are a pain
// a struct: fast. Don't have to keep track of indexes. Can't iterate
uint32_t stateForError;
error_state_struct error_state;

error_state_struct get_error_state()
{
  // provides read-only access to the error states
  return error_state;
}

void run_checks();

bool check_all_errors()
{
  // This is ugly but for now just manually list everything out here
  run_checks();
  return error_state.fcvolt_high || error_state.fcvolt_low ||
         error_state.capvolt_high || error_state.capvolt_low ||
         error_state.fccurr_high || error_state.capcurr_high ||
         error_state.press_high || error_state.press_low ||
         error_state.over_temp || error_state.relays_shorted ||
         error_state.stop || error_state.h2_ok ||
         error_state.estop1 || error_state.estop2;
}

bool expect_low_voltage(uint32_t fc_state)
{
  return (fc_state == FC_STANDBY) || 
         (fc_state == FC_PRESSURIZE) ||
         (fc_state == FC_START_UP_PURGE) ||
         (fc_state == FC_CHARGE) ||
         (fc_state == FC_SHUTDOWN) || 
         (fc_state == FC_ALARM);
}

bool expect_low_cap_voltage(uint32_t fc_state)
{
  return (fc_state == FC_STANDBY) || 
         (fc_state == FC_PRESSURIZE) ||
         (fc_state == FC_START_UP_PURGE) ||
         (fc_state == FC_CHARGE) ||
         (fc_state == CAP_CHARGE) ||
         (fc_state == FC_SHUTDOWN) || 
         (fc_state == FC_ALARM);
}

bool expect_low_pressure(uint32_t fc_state)
{
  return (fc_state == FC_STANDBY) || 
         (fc_state == FC_PRESSURIZE) ||
         (fc_state == FC_START_UP_PURGE) ||
         (fc_state == FC_SHUTDOWN) || 
         (fc_state == FC_ALARM);
}

void run_checks()
{
  // Loop and check alert conditions at 10 Hz
  if (get_analog_values().fcvolt > FCVOLT_MAX)
  {
    error_state.fcvolt_high = true;
  }
  if ((get_analog_values().fcvolt < FCVOLT_MIN) && !expect_low_voltage(get_fc_state()))
  {
    error_state.fcvolt_low = true;
  }
  if (get_analog_values().capvolt > CAPVOLT_MAX)
  {
    error_state.capvolt_high = true;
  }
  if ((get_analog_values().capvolt < CAPVOLT_MIN) && !expect_low_cap_voltage(get_fc_state()))
  {
    error_state.capvolt_low = true;
  }
  if (get_analog_values().fccurr > MAX_FC_CURR)
  {
    error_state.fccurr_high = true;
  }
  // if (get_analog_values().capcurr > MAX_CAP_CURR) Please Fix me
  // {
  //   error_state.capcurr_high = true;
  // }
  if (get_analog_values().press1 > PRESSURE_MAX)
  {
    error_state.press_high = true;
  }
  if ((get_analog_values().press1 < PRESSURE_MIN) && !expect_low_pressure(get_fc_state()))
  {
    error_state.press_low = true;
  }
  if ((get_analog_values().fctemp1 > TEMP_MAX))
  {
    error_state.over_temp = true;
  }
  error_state.relays_shorted = get_relay_conflict() | error_state.relays_shorted;
  error_state.stop = !stop | error_state.stop;
  // error_state.h2_ok = !h2_ok | error_state.h2_ok; Doesn't seem to be sent by H2_Board. Please Fix
  error_state.estop1 = !estop1 | error_state.estop1;
  error_state.estop2 = !estop2 | error_state.estop2;
}
