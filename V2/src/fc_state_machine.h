#ifndef FC_STATEMACHINE_H
#define FC_STATMACHINE_H

// Uncomment the line below to enable the test state.
#define ENABLE_FC_TEST

// todo - fill in rest of states
#define FC_INIT 0
#define FC_STARTUP 1
#define FC_FAULT 2
#define FC_TEST 10
#include <stdint.h>

uint32_t get_fc_state();
void fc_state_machine_thread();

#endif
