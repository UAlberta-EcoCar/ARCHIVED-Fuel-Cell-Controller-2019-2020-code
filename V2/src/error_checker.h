#ifndef ERROR_CHECKER_H
#define ERROR_CHECKER_H

struct error_state_struct {
   bool fcvolt_high;
   bool fcvolt_low;
};

error_state_struct get_error_state();

bool check_all_errors();

void error_checker_thread();

#endif