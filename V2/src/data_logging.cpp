#include "mbed.h"

#include <pin_defs.h>
#include <analogs.h>
#include <error_checker.h>
#include <fc_state_machine.h>

#define FTDI_BAUD 115200
#define BLUE_BAUD 115200
#define OL_BAUD 115200

Timer datatimer;
void printData(char* buffer);

//Object declarations
//Serial pc(USBTX, USBRX); // todo - remove this since code will freeze if usb isn't connected
Serial ftdi(FTDI_TX, FTDI_RX);
Serial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX, BLUE_BAUD);
Serial open_log(OPEN_LOG_TX, OPEN_LOG_RX);

char buffer [100];

void data_logging_thread() {
    // Dalin's cpp magic made this a lot nicer but oh well.
    while (true) {
        // State
        if(get_fc_state() == FC_STANDBY){sprintf(buffer,"\n\nState: FC_STANDBY\n");}
        else if (get_fc_state() == FC_PRESSURIZE){sprintf(buffer,"\n\nState: FC_PRESSURIZE\n");}
        else if (get_fc_state() == FC_START_UP_PURGE){sprintf(buffer,"\n\nState: FC_START_UP_PURGE\n");}
        else if (get_fc_state() == FC_CHARGE){sprintf(buffer,"\n\nState: FC_CHARGE\n");}
        else if (get_fc_state() == CAP_CHARGE){sprintf(buffer,"\n\nState: CAP_CHARGE\n");}
        else if (get_fc_state() == FC_RUN){sprintf(buffer,"\n\nState: FC_RUN\n");}
        else if (get_fc_state() == FC_PURGE){sprintf(buffer,"\n\nState: FC_PURGE\n");}
        else if (get_fc_state() == FC_SHUTDOWN){sprintf(buffer,"\n\nState: FC_SHUTDOWN\n");}
        else if (get_fc_state() == FC_ALARM){sprintf(buffer,"\n\nState: FC_ALARM\n");}
        else if (get_fc_state() == FC_TEST){sprintf(buffer,"\n\nState: FC_TEST\n");}
        else {sprintf(buffer,"\n\nState: UNKNOWN STATE?!\n");}
        printData(buffer);

        sprintf(buffer, "Purge Count: %-5u|", get_purge_count());
        printData(buffer);

        sprintf(buffer, "  Pressure: %-10.1f|", get_analog_values().press1);
        printData(buffer);

        if (check_all_errors())
        {
          sprintf(buffer, "   Error: %-3s\n", "YES");
        }
        else
        {
          sprintf(buffer, "   Error: %-3s\n", "NO");
        }
        printData(buffer);

        sprintf(buffer, "Purge Timer: %-2.2f |", get_purge_timer());
        printData(buffer);

        sprintf(buffer, "  Temperature: %-7.1f|", get_analog_values().fctemp1);
        printData(buffer);

        sprintf(buffer, "   Error: ");
        printData(buffer);

        if (check_all_errors())
        {
          if (get_error_state().capvolt_high){
            sprintf(buffer, "Cap Voltage High, ");
            printData(buffer);
          }
          if (get_error_state().capvolt_low){
            sprintf(buffer, "Cap Voltage Low, ");
            bluetooth.puts(buffer);
          }
          if (get_error_state().fccurr_high){
            sprintf(buffer, "FC Current High, ");
            printData(buffer);
          }
          if (get_error_state().capcurr_high){
            sprintf(buffer, "Cap Current High, ");
            printData(buffer);
          }
          if (get_error_state().press_high){
            sprintf(buffer, "Over Pressure, ");
            printData(buffer);
          }
          if (get_error_state().press_low){
            sprintf(buffer, "Under Pressure, ");
            printData(buffer);
          }
          if (get_error_state().fcvolt_high){
            sprintf(buffer, "FC Voltage High, ");
            printData(buffer);
          }
          if (get_error_state().fcvolt_low){
            sprintf(buffer, "FC Voltage Low, ");
            printData(buffer);
          }
          if (get_error_state().relays_shorted){
            sprintf(buffer, "Relays are Shorted, ");
            printData(buffer);
          }
          if (get_error_state().over_temp){
            sprintf(buffer, "Over Temperature, ");
            printData(buffer);
          }
          if (get_error_state().stop){
            sprintf(buffer, "Stop, ");
            printData(buffer);
          }
          if (get_error_state().h2_ok){
            sprintf(buffer, "h2 ok, ");
            printData(buffer);
          }
          if (get_error_state().estop1){
            sprintf(buffer, "estop1, ");
            printData(buffer);
          }
          if (get_error_state().estop2){
            sprintf(buffer, "estop2, ");
            printData(buffer);
          }
        }

        sprintf(buffer, "\n\nFC Voltage: %-5.1f|", get_analog_values().fcvolt);
        printData(buffer);

        sprintf(buffer, "  Cap Voltage: %-8.1f|", get_analog_values().capvolt);
        printData(buffer);

        sprintf(buffer, "  Motor Voltage: %-3.1f\n", get_analog_values().motorvolt);
        printData(buffer);

        sprintf(buffer, "FC Current: %-5.1f|", get_analog_values().fccurr);
        printData(buffer);

        sprintf(buffer, "  Cap Current: %-8.1f|", get_analog_values().capcurr);
        printData(buffer);

        sprintf(buffer, "  Motor Current: %-5.1f\n", get_analog_values().motorcurr);
        printData(buffer);

        sprintf(buffer, "\n  Relays Open (OFF): ");
        printData(buffer);
        if (!start_r.read()){
          sprintf(buffer, "START_R, ");
          printData(buffer);
        }
        if (!motor_r.read()){
          sprintf(buffer, "MOTOR_R, ");
          printData(buffer);
        }
        if (!charge_r.read()){
          sprintf(buffer, "CHARGE_R, ");
          printData(buffer);
        }
        if (!cap_r.read()){
          sprintf(buffer, "CAP_R, ");
          printData(buffer);
        }
        if (!fcc_r.read()){
          sprintf(buffer, "FCc_R, ");
          printData(buffer);
        }

        sprintf(buffer, "\n Relays Closed (ON): ");
        printData(buffer);
        if (start_r.read()){
          sprintf(buffer, "START_R, ");
          printData(buffer);
        }
        if (motor_r.read()){
          sprintf(buffer, "MOTOR_R, ");
          printData(buffer);
        }
        if (charge_r.read()){
          sprintf(buffer, "CHARGE_R, ");
          printData(buffer);
        }
        if (cap_r.read()){
          sprintf(buffer, "CAP_R, ");
          printData(buffer);
        }
        if (fcc_r.read()){
          sprintf(buffer, "FCC_R, ");
          printData(buffer);
        }

        sprintf(buffer, "\n   Valves Open (ON): ");
        printData(buffer);
        if (supply_v.read()){
          sprintf(buffer, "SUPPLY_V, ");
          printData(buffer);
        }
        if (purge_v.read()){
          sprintf(buffer, "SUPPLY_V, ");
          printData(buffer);
        }

        sprintf(buffer, "\nValves Closed (OFF): ");
        printData(buffer);
        if (!supply_v.read()){
          sprintf(buffer, "SUPPLY_V, ");
          printData(buffer);
        }
        if (!purge_v.read()){
          sprintf(buffer, "PURGE_V, ");
          printData(buffer);
        }

        sprintf(buffer, "\n____________________________________________________________________________");
        printData(buffer);

        ThisThread::sleep_for(2000);
    }
}

void printData(char* buffer)
{
  ThisThread::sleep_for(0.1);
  ftdi.puts(buffer);
  ThisThread::sleep_for(0.1);
  bluetooth.puts(buffer);
  ThisThread::sleep_for(0.1);
  open_log.puts(buffer);
  ThisThread::sleep_for(0.1);
}
/* Datalogging output formatting template
State: RUN
Purge Count:  0    |   Error: YES    |   Pressure: 1.5
Purge Timer: 18.01 |   Error: Stop   |   Temperature: 36.0

FC Voltage: 43.0   |   Cap Voltage:  29.6   |   Motor Voltage: 23.5
FC Current: -5.6   |   Cap Current: -57.6   |   Motor Current: -1.7

  Relays Open (OFF): FCC_R
 Relays Closed (ON): START_R, MOTOR_R, CHARGE_R, CAP_R
   Valves Open (ON):
Valves Closed (OFF): SUPPLY_V, PURGE_V
*/
