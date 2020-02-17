#include "mbed.h"

#include <analogs.h>
#include <error_checker.h>
#include <data_logging.h>
#include <fc_state_machine.h>
#include <fan.h>

Thread analog_read_thr;
Thread data_logging_thr;
Thread fc_state_machine_thr;
Thread fan_thr;

int main() {
    analog_read_thr.start(analog_read_thread);

    data_logging_thr.start(data_logging_thread);

    fc_state_machine_thr.start(fc_state_machine_thread);

    fan_thr.start(fan_thread);

    while (true) {
        wait_us(0.5);
    }
}

//In your program you need to set PB_10 and PB_11 as high impedance. TX is PD_8, RX is PD_9
