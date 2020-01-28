#include "mbed.h"

#include <pin_defs.h>
#include <analogs.h>
#include <error_checker.h>
#include <fc_state_machine.h>

DigitalOut led3(LED3);

//Serial pc(USBTX, USBRX); // todo - remove this since code will freeze if usb isn't connected
Serial ftdi(FTDI_TX, FTDI_RX);
Serial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
Serial open_log(OPEN_LOG_TX, OPEN_LOG_RX);

char buffer [100];

void data_logging_thread() {
    // Dalin's cpp magic made this a lot nicer but oh well.
    while (true) {

        sprintf(buffer, "{State: %lu}\n", get_fc_state());
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        if (check_all_errors()){
          sprintf(buffer, "{Error?: %3s}\n", "YES");
        }
        else {sprintf(buffer, "{Error?: %3s}\n", "NO");}
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        sprintf(buffer, "{FC Voltage: %2.1f}\n", get_analog_values().fcvolt);
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        sprintf(buffer, "{FC Current: %2.1f}\n", get_analog_values().fccurr);
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        sprintf(buffer, "{Cap Voltage: %2.1f}\n", get_analog_values().capvolt);
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        sprintf(buffer, "{Cap Current: %2.1f}\n", get_analog_values().capcurr);
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        sprintf(buffer, "{Motor Voltage: %2.1f}\n", get_analog_values().motorvolt);
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        sprintf(buffer, "{Motor Current: %2.1f}\n", get_analog_values().motorcurr);
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        ThisThread::sleep_for(3000);
        led3 = !led3;
    }
}
