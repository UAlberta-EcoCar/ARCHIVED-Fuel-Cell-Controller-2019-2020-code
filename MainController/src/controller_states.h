#ifndef CONTROLLER_STATES_H
#define CONTROLLER_STATES_H

void standby_state();

void fan_spool_up();
void start_purge();
void start_end();
void start_charge();

void run_setup();
void purge();
void test_state();

void shutdown_state();

#endif
