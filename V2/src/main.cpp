#include "mbed.h"

#include <analogs.h> 
#include <error_checker.h>
#include <data_logging.h>
#include <fc_state_machine.h>

// Watch dog timer timeout
const uint32_t TIMEOUT_MS = 1000;

Thread analog_read_thr;
Thread error_checker_thr;
Thread data_logging_thr;
Thread fc_state_machine_thr;

int main() {

    // start watchdog timer
    Watchdog &watchdog = Watchdog::get_instance();
    watchdog.start(min(TIMEOUT_MS, get_max_timeout ()));
    
    // start threads
    analog_read_thr.start(analog_read_thread);
    error_checker_thr.start(error_checker_thread);
    data_logging_thr.start(data_logging_thread);
    fc_state_machine_thr.start(fc_state_machine_thread);

    while (true) {
        // Reset watchdog timer
        watchdog.kick();  // this is a little violent
        
        wait(0.5);
    }
}
