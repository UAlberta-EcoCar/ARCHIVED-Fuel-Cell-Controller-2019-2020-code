#ifndef FC_STATEMACHINE_H
#define FC_STATMACHINE_H

// Uncomment the line below to enable the test state.
//#define ENABLE_FC_TEST

#define FC_STANDBY 0
#define FC_CHARGE 1
#define FC_RUN 2
#define FC_SHUTDOWN 3
#define FC_ALARM 4
#define FC_TEST 10
#include <stdint.h>

uint32_t get_fc_state();
uint32_t get_purge_count();
bool get_relay_conflict();
void fc_state_machine_thread();

#endif
