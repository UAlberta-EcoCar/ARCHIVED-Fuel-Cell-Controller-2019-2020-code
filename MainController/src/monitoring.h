#ifndef MONITORING_H
#define MONITORING_H

extern EventFlags controller_flags;
extern Semaphore fan_spooled, startup_purge;

void monitoring_thread();

#endif
