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
   bool stop;
   bool h2_ok;
   bool estop1;
   bool estop2;
};

error_state_struct get_error_state();

bool check_all_errors();

#endif
