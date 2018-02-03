#ifndef MONITORING_H
#define MONITORING_H

extern EventFlags controller_flags;
// First Byte
#define FINISHED_EXCUTION_FLAG 0x1
#define START_EVENT_FLAG 0x2
#define RUN_EVENT_FLAG 0x4
#define PURGE_EVENT_FLAG 0x8
#define SHUT_EVENT_FLAG 0x10
#define ALARM_EVENT_FLAG 0x20
#define CLEAR_EVENT_FLAG 0xf7

// Second Byte
#define FAN_SHUTDOWN_FLAG 0x100
#define FAN_MIN_FLAG 0x200
#define FAN_MAX_FLAG 0x400
#define FAN_PID_FLAG 0x800
#define CLEAR_FAN_FLAG 0xff00

// Thrid Byte
#define FAN_SPOOLED_FLAG 0x10000
#define START_PURGE_FLAG 0x20000
#define CLEAR_SIGNAL_FLAG 0xff0000

// Fourth Byte
#define START_BUTTON_PRESSED 0x1000000


void monitoring_thread();

#endif
