#ifndef CONTROLLER_EVENT_QUEUE_H
#define CONTROLLER_EVENT_QUEUE_H

void contoller_event_queue_thread();

void alarm_state();
void start_state();
void run_state();
void shut_state();
void purge();

extern EventQueue cont_queue;

#endif
