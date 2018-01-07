#ifndef MONITORING_H
#define MONITORING_H

extern Semaphore fan_spooled, startup_purge;
extern vector<Sensor*> an_in_vec;
extern vector<Integrator*> int_vec;
extern vector<Sensor*>::iterator an_iter;
extern vector<Integrator*>::iterator int_iter;

void monitoring_thread();

#endif
