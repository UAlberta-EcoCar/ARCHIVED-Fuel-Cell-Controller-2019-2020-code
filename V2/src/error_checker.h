#ifndef ERROR_CHECKER_H
#define ERROR_CHECKER_H

struct error_state_struct {
   bool fcvolt_high;
   bool fcvolt_low;
   bool capvolt_high;
   bool capvolt_low;
   bool fccurr_high;
   bool capcurr_high;
   bool press_high;
   bool press_low;
   bool relays_shorted;
   bool over_temp;
};

error_state_struct get_error_state();

bool check_all_errors();

void error_checker_thread();

bool expect_low_voltage();
bool expect_low_pressure();
bool expect_low_cap_voltage();

#endif
