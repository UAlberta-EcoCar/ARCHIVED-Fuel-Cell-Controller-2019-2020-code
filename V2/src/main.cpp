#include "mbed.h"

#include <analogs.h> 
#include <error_checker.h>
#include <data_logging.h>
#include <fc_state_machine.h>

DigitalOut led1(LED1);  // todo - remove this

Thread analog_read_thr;
Thread error_checker_thr;
Thread data_logging_thr;
Thread fc_state_machine_thr;

int main() {
    analog_read_thr.start(analog_read_thread);
    
    error_checker_thr.start(error_checker_thread);
    
    data_logging_thr.start(data_logging_thread);

    fc_state_machine_thr.start(fc_state_machine_thread);

    while (true) {
        led1 = !led1; // todo - remove this
        wait(0.5);
    }
}
