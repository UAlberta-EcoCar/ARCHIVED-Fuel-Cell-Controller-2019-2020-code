#ifndef MONITORING_H
#define MONITORING_H

extern Semaphore fan_spooled, startup_purge;

void monitoring_thread();

#endif
