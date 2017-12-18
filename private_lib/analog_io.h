#ifndef ANALOG_IO_H
#define ANALOG_IO_H

float get_capvolt(void);
float get_fccurr(void);
float get_fcvolt(void);
float get_capcurr(void);
float get_fcpres(void);

double get_fc_coulumbs(void);
double get_fc_joules(void);
double get_cap_coulumbs(void);
double get_cap_joules(void);

void set_capvolt(void);
void set_fccurr(void);
void set_fcvolt(void);
void set_capcurr(void);
void set_fcpres(void);

void set_fc_coulumbs(float dt);
void set_fc_joules(float dt);
void set_cap_coulumbs(float dt);
void set_cap_joules(float dt);

#endif
