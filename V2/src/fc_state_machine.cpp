#include "mbed.h"
#include "rtos.h"

#include <Timer.h>
#include <math.h>
#include <fc_state_machine.h>
#include <error_checker.h>
#include <pin_defs.h>
#include <analogs.h>

#define CAP_THRESHOLD 20 // Move to another file, if they aren't already.

void update_leds(void);

#ifdef ENABLE_FC_TEST
uint32_t state = FC_TEST;
#else
uint32_t state = FC_STANDBY;
#endif

DigitalIn start(START);
DigitalIn button(BUTTON);

// Valves.
DigitalOut supply_v(SUPPLY_V);
DigitalOut purge_v(PURGE_V);
DigitalOut other1_v(VALVE3);
DigitalOut other2_v(VALVE4);

// Relays.
DigitalOut start_r(START_R);
DigitalOut motor_r(MOTOR_R);
DigitalOut charge_r(CHARGE_R);
DigitalOut cap_r(CAP_R);
DigitalOut fcc_r(FCC_R);

// LEDs.
DigitalOut alarm_led(ALARM_LED);
DigitalOut debug_led(DEBUG_LED);
DigitalOut shut_led(SHUT_LED);
DigitalOut run_led(RUN_LED);
DigitalOut start_led(START_LED);

Timer purgeTimer;
uint32_t purgeCount = 0;

uint32_t get_fc_state() // Read-only access to state variable.
{
    return state;
}
uint32_t get_purge_count() // Read-only access to purge count.
{
  return purgeCount;
}

bool get_relay_conflict()
{
  if (start_r&charge_r){
    return true;
  }
  else{
    return false;
  }
}

void fc_state_machine_thread()
{
    fcc_r.write(true); // Solid state relay.

    while (true) {
        if (check_all_errors() && (state!=FC_STANDBY) && (state!=FC_TEST) && (state!=FC_SHUTDOWN) && (state!=FC_CHARGE)) {
            state = FC_ALARM;
        }
        if (state == FC_TEST){
          ThisThread::sleep_for(50); // Add test state code here, if desired.
          purgeTimer.start();

          if (purgeTimer.read()>15) {
            purgeTimer.stop();

            purge_v.write(true);
            ThisThread::sleep_for(200);
            purge_v.write(false);

            purgeTimer.reset();
            purgeTimer.start();
            purgeCount = purgeCount+1;
          }
        }
        else if (FC_STANDBY == state) // Wait until start button is pressed.
        {
            if (button)
            {
                purge_v.write(false); // Perform start purge.
                start_r.write(true);
                motor_r.write(false);
                charge_r.write(false);
                cap_r.write(false);
                supply_v.write(true);
                ThisThread::sleep_for(1000);
                purge_v.write(true);
                ThisThread::sleep_for(200);
                purge_v.write(false);

                supply_v.write(true); // Charge state setup.
                start_r.write(false);
                motor_r.write(false);
                charge_r.write(true);
                cap_r.write(false);
                state = FC_CHARGE;
                update_leds();
            }
        }
        else if (FC_CHARGE == state)
        {
            if (get_analog_values().capvolt >= CAP_THRESHOLD) // Leave charge state.
            {
                supply_v.write(true);
                purge_v.write(false);
                start_r.write(false);
                motor_r.write(true);
                charge_r.write(false);
                cap_r.write(true);
                state = FC_RUN;
                update_leds();
            }
        }
        else if (FC_RUN == state)
        {
          if (purgeTimer.read()>3*60) {
            purgeTimer.stop();

            purge_v.write(true);
            ThisThread::sleep_for(200);
            purge_v.write(false);

            purgeTimer.reset();
            purgeTimer.start();
            purgeCount = purgeCount+1;
          }

            if (start)
            {
                supply_v.write(false);
                purge_v.write(false);
                start_r.write(false);
                motor_r.write(false);
                charge_r.write(false);
                cap_r.write(false);
                state = FC_SHUTDOWN;
                update_leds();
            }
        }
        else if (FC_SHUTDOWN == state)
        {
            // TODO Figure out so we can start/stop with start button.
        }
        else if (FC_ALARM == state) // Turn off all digital outputs.
        {
            update_leds();
            supply_v.write(false);
            purge_v.write(false);
            start_r.write(false);
            motor_r.write(false);
            charge_r.write(false);
            cap_r.write(false);
        }
        else
        {
            // Fault since invalid state entered.
            state = FC_ALARM;
        }

        ThisThread::sleep_for(0.05);
    }
}

void update_leds()
{
    alarm_led.write(false);
    shut_led.write(false);
    start_led.write(false);
    run_led.write(false);
    uint32_t state = get_fc_state();

    switch (state)
    {
        case FC_ALARM:
            alarm_led.write(true);
            break;
        case FC_SHUTDOWN:
            shut_led.write(true);
            break;
        case FC_STANDBY:
            start_led.write(true);
            break;
        case FC_CHARGE:
            start_led.write(true);
            run_led.write(true);
            break;
        case FC_RUN:
            run_led.write(true);
        default:
            break; // Shouldn't happen.
    }
}
