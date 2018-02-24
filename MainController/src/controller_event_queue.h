#ifndef CONTROLLER_EVENT_QUEUE_H
#define CONTROLLER_EVENT_QUEUE_H

void contoller_event_queue_thread();

void start_state();
void start_purge();
void fc_charge_entry();
void fc_charge_exit();
void charge_state();
void cap_charge_entry();
void cap_charge_exit();
void run_state();
void purge();
void shut_state();
void alarm_state();
void update_leds();
void test();

extern EventQueue cont_queue;

#endif
