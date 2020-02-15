#ifndef FC_STATEMACHINE_H
#define FC_STATMACHINE_H

// Uncomment the line below to enable the test state.
#define ENABLE_FC_TEST

#define FC_STANDBY 0
#define FC_PRESSURIZE 1
#define FC_START_PURGE 2
#define FC_CHARGE 3
#define FC_RUN 4
#define FC_SHUTDOWN 5
#define FC_ALARM 6
#define FC_TEST 7

uint32_t get_fc_state();
void fc_state_machine_thread();

#endif
