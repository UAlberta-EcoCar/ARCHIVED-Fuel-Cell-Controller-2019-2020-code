#ifndef ANALOGS_H
#define ANALOGS_H

struct analog_values_struct {
   float fcvolt;
   float motorvolt;
   float capvolt;
   float fccurr;
   float motorcurr;
   float capcurr;
};

analog_values_struct get_analog_values();

void analog_read_thread();

#endif