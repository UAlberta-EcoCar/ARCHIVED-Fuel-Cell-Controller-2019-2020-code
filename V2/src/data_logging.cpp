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

        sprintf(buffer, "\n\n\n\n\n\n\n\n\n\n\n\n\n{State: %lu}     ", get_fc_state());
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
            sprintf(buffer, "{Error: Cap Voltage High}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().capvolt_low){
            sprintf(buffer, "{Error: Cap Voltage Low}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().fccurr_high){
            sprintf(buffer, "{Error: FC Current High}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().capcurr_high){
            sprintf(buffer, "{Error: Cap Current High}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().press_high){
            sprintf(buffer, "{Error: Over Pressure}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().press_low){
            sprintf(buffer, "{Error: Under Pressure}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().fcvolt_low){
            sprintf(buffer, "{Error: FC Voltage Low}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().relays_shorted){
            sprintf(buffer, "{Error: Relays are Shorted}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().over_temp){
            sprintf(buffer, "{Error: Over Temperature}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().stop){
            sprintf(buffer, "{Error: Stop}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().h2_ok){
            sprintf(buffer, "{Error: h2 ok}n");
            bluetooth.puts(buffer);
          }
          if (get_error_state().estop1){
            sprintf(buffer, "{Error: estop1}");
            bluetooth.puts(buffer);
          }
          if (get_error_state().estop2){
            sprintf(buffer, "{Error: estop2}");
            bluetooth.puts(buffer);
          }
        }
        else {
          sprintf(buffer, "{Error?: %3s}\n", "NO");
          bluetooth.puts(buffer);
        }
        
        sprintf(buffer, "\n{FC Voltage: %2.1f}     ", get_analog_values().fcvolt);
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{FC Current: %2.1f}\n", get_analog_values().fccurr);
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{Cap Voltage: %2.1f}    ", get_analog_values().capvolt);
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{Cap Current: %2.1f}\n", get_analog_values().capcurr);
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{Motor Voltage: %2.1f}  ", get_analog_values().motorvolt);
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{Motor Current: %2.1f}\n", get_analog_values().motorcurr);
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{Temperature: %2.1f}  ", get_analog_values().fctemp1);
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{Pressure: %2.1f}\n", get_analog_values().press1);
        bluetooth.puts(buffer);

        sprintf(buffer, "{START_R: %d}", start_r.read());
        bluetooth.puts(buffer);

        sprintf(buffer, "{MOTOR_R: %d}", motor_r.read());
        bluetooth.puts(buffer);

        sprintf(buffer, "{CHARGE_R: %d}", charge_r.read());
        bluetooth.puts(buffer);

        sprintf(buffer, "{CAP_R: %d}", cap_r.read());
        bluetooth.puts(buffer);

        sprintf(buffer, "{FCC_R: %d}\n", fcc_r.read());
        bluetooth.puts(buffer);

        sprintf(buffer, "{SUPPLY_V: %d}", supply_v.read());
        bluetooth.puts(buffer);

        sprintf(buffer, "{PURGE_V: %d}\n", purge_v.read());
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{START: %d}", start.read());
        bluetooth.puts(buffer);
        
        sprintf(buffer, "{BUTTON: %d}", button.read());
        bluetooth.puts(buffer);

        sprintf(buffer, "{BLUE BUTTON: %d}\n\n", blue_button.read());
        bluetooth.puts(buffer);
        
        ThisThread::sleep_for(500);
    }
}
