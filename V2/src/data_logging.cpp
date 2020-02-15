#include "mbed.h"

#include <pin_defs.h>
#include <analogs.h>
#include <error_checker.h>
#include <fc_state_machine.h>

#define FTDI_BAUD 115200
#define BLUE_BAUD 115200
#define OL_BAUD 115200

Timer datatimer;

//Object declarations
//Serial pc(USBTX, USBRX); // todo - remove this since code will freeze if usb isn't connected
Serial ftdi(FTDI_TX, FTDI_RX);
Serial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX, BLUE_BAUD);
Serial open_log(OPEN_LOG_TX, OPEN_LOG_RX);

char buffer [100];

void data_logging_thread() {
    // Dalin's cpp magic made this a lot nicer but oh well.
    while (true) {

        sprintf(buffer, "{State: %lu}     ", get_fc_state());
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        sprintf(buffer, "{Purge Count: %ld}     ", get_purge_count());
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
          sprintf(buffer, "{Error: %3s}\n", "YES");
          bluetooth.puts(buffer);


          if (get_error_state().capvolt_high){
            sprintf(buffer, "{Error: Cap Voltage High}\n");
          }
          else if (get_error_state().capvolt_low){
            sprintf(buffer, "{Error: Cap Voltage Low}\n");
          }
          else if (get_error_state().fccurr_high){
            sprintf(buffer, "{Error: FC Current High}\n");
          }
          else if (get_error_state().capcurr_high){
            sprintf(buffer, "{Error: Cap Current High}\n");
          }
          else if (get_error_state().press_high){
            sprintf(buffer, "{Error: Over Pressure}\n");
          }
          else if (get_error_state().press_low){
            sprintf(buffer, "{Error: Under Pressure}\n");
          }
          else if (get_error_state().fcvolt_low){
            sprintf(buffer, "{Error: FC Voltage Low}\n");
          }
          else if (get_error_state().relays_shorted){
            sprintf(buffer, "{Error: Relays are Shorted}\n");
          }
          else if (get_error_state().over_temp){
            sprintf(buffer, "{Error: Over Temperature}\n");
          }
          else {
            sprintf(buffer, "{Error: None of the errors?}\n");
          }

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

        sprintf(buffer, "{FC Voltage: %2.1f}     ", get_analog_values().fcvolt);
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

        sprintf(buffer, "{Cap Voltage: %2.1f}    ", get_analog_values().capvolt);
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

        sprintf(buffer, "{Motor Voltage: %2.1f}  ", get_analog_values().motorvolt);
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

        sprintf(buffer, "{Temperature: %2.1f}  ", get_analog_values().fctemp1);
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        sprintf(buffer, "{Pressure: %2.1f}\n\n", get_analog_values().press1);
        //ThisThread::sleep_for(0.1);
        // pc.puts(buffer);
        ThisThread::sleep_for(0.1);
        ftdi.puts(buffer);
        ThisThread::sleep_for(0.1);
        bluetooth.puts(buffer);
        ThisThread::sleep_for(0.1);
        open_log.puts(buffer);
        ThisThread::sleep_for(0.1);

        ThisThread::sleep_for(3000); // 7s

    }
}
