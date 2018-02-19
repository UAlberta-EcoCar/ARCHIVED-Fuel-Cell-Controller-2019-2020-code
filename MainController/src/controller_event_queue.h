#ifndef CONTROLLER_EVENT_QUEUE_H
#define CONTROLLER_EVENT_QUEUE_H

void contoller_event_queue_thread();

void alarm_state();
void start_state();
void start_purge();
void start_res_start();
void start_res_end();
void charge_state();
void charge_start();
void charge_stop();
void run_state();
void shut_state();
void purge();

extern EventQueue cont_queue;

#endif
