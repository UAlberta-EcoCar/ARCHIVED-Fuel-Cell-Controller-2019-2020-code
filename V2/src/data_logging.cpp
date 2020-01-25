#include "mbed.h"

#include <pin_defs.h>
#include <analogs.h>
#include <error_checker.h>
#include <fc_state_machine.h>

DigitalOut led3(LED3);

Serial pc(USBTX, USBRX); // todo - remove this since code will freeze if usb isn't connected
Serial ftdi(FTDI_TX, FTDI_RX);
Serial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
Serial open_log(OPEN_LOG_TX, OPEN_LOG_RX);

char buffer [100];

void data_logging_thread() {
    // Dalin's cpp magic made this a lot nicer but oh well.
    while (true) {
        // todo - complete this

        sprintf(buffer, "{State: %lu}\n", get_fc_state());
        ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        led3 = !led3;
    }
}
