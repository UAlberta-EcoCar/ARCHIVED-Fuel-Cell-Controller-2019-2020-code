#ifndef CONTROLLER_EVENT_QUEUE_H
#define CONTROLLER_EVENT_QUEUE_H

void contoller_event_queue_thread();

void standby_state();

void fan_spool_up();
void start_purge();
void start_end();
void start_charge();

void run_setup();
void purge();
void test_state();

void shutdown_state();
extern EventQueue cont_queue;

#endif
