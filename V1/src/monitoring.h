#ifndef MONITORING_H
#define MONITORING_H

extern EventFlags controller_flags;

// First Byte: Signal Flags
#define SIGNAL_STARTSETUPCOMPLETE 0x1
#define SIGNAL_CHARGESETUPCOMPLETE 0x10
#define SIGNAL_CHARGESTARTED 0x20
#define SIGNAL_CHARGECOMPLETED 0x40
#define SIGNAL_STATETRANSITION 0x80
#define CLEAR_SIGNAL_FLAG 0xff

// Second Byte: Fan Flags
#define FAN_SHUTDOWN_FLAG 0x100
#define FAN_MIN_FLAG 0x200
#define FAN_MAX_FLAG 0x400
#define FAN_PID_FLAG 0x800
#define CLEAR_FAN_FLAG 0xff00

void state_monitoring();
void monitoring_thread();

#endif
