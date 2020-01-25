#ifndef FC_STATEMACHINE_H
#define FC_STATMACHINE_H

uint32_t get_fc_state();

// todo - fill in rest of states
#define FC_INIT 0
#define FC_STARTUP 1
#define FC_FAULT 2

void fc_state_machine_thread();

#endif
