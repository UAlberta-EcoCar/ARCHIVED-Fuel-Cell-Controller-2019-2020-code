#ifndef DATALINK_H
#define DATALINK_H

#include "Classes/SerialPrinter.h"

void datalink_thread();
void error_logging();
extern SerialPrinter fdti_printer;

#endif