#ifndef FC_STATEMACHINE_H
#define FC_STATMACHINE_H

// Uncomment the line below to enable the test state.
//#define ENABLE_FC_TEST

#define FC_STANDBY 0
#define FC_PRESSURIZE 1
#define FC_START_UP_PURGE 2
#define FC_CHARGE 3
#define CAP_CHARGE 4
#define FC_RUN 5
#define FC_PURGE 6
#define FC_SHUTDOWN 7
#define FC_ALARM 8
#define FC_TEST 9
#include <stdint.h>

uint32_t get_fc_state();
uint32_t get_purge_count();
float get_purge_timer();
bool get_relay_conflict();
void fc_state_machine_thread();

#endif
