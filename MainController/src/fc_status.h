#ifndef FC_STATUS_H
#define FC_STATUS_H

//signals that send messages between threads
#define STANDBY_STATE 0
#define START_STATE 3
#define SHUTDOWN_STATE 1
#define RUN_STATE 6
#define ALARM_STATE 4

int get_fc_status(void);
void set_fc_status(int val);

#endif
