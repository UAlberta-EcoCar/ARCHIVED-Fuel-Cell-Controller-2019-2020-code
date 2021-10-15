#include "mbed.h"
#include "rtos.h"

#include <Timer.h>
#include <math.h>
#include <fc_state_machine.h>
#include <error_checker.h>
#include <error_thresholds.h>
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
DigitalIn button(BUTTON); // This button is broken please fix
DigitalIn blue_button(BLUE_BUTTON);

// Relays.
DigitalOut start_r(START_R);
DigitalOut motor_r(MOTOR_R);
DigitalOut charge_r(CHARGE_R);
DigitalOut cap_r(CAP_R);
DigitalOut fcc_r(FCC_R);

// Valves.
DigitalOut supply_v(SUPPLY_V);
DigitalOut purge_v(PURGE_V);
DigitalOut other1_v(VALVE3);
DigitalOut other2_v(VALVE4);

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
float get_purge_timer()
{
    return purgeTimer.read();
}
bool get_relay_conflict()
{
    if (start_r & charge_r)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void fc_state_machine_thread()
{
    fcc_r.write(true); // Solid state relay.
    update_leds();

    bool start_purge_performed = false;

    while (true)
    {
        if (check_all_errors())
        {
            state = FC_ALARM;
        }

        if (FC_STANDBY == state) // Wait until start button is pressed.
        {
            if (blue_button.read())
            {
                state = FC_PRESSURIZE;
            }
        }
        else if (FC_PRESSURIZE == state)
        {
            /*
            Open supply valve and wait for stack to pressurize.
            */
            supply_v.write(true);
            if (get_analog_values().press1 > PRESSURE_MIN)
            { // Make me a constant
                state = FC_START_UP_PURGE;
            }
        }
        else if (FC_START_UP_PURGE == state)
        {
            /*
            Perform startup purge with start resistors connected
            */
            if (!start_purge_performed)
            {
                purge_v.write(true);
                start_r.write(true);
                ThisThread::sleep_for(200);
                start_r.write(false);
                purge_v.write(false);
                start_purge_performed = true;
            }
            // Wait for pressure to recover
            if (get_analog_values().press1 > PRESSURE_MIN)
            {
                state = FC_CHARGE;
            }
        }
        else if (FC_CHARGE == state)
        {
            /*
            Let fc voltage recover after purge
            */
            if (get_analog_values().fcvolt >= 20)
            {
                state = CAP_CHARGE;
                purgeTimer.start();
                charge_r.write(true);
            }
        }
        else if (CAP_CHARGE == state)
        {
            /*
            Charge caps to __ V using the startup resistors. Then charge
            to __ V without them
            */
            if (get_analog_values().capvolt >= 30)
            { // make me a constant
                // Disconnect charge_r then connect cap_r so charge goes faster
                charge_r.write(false);
                ThisThread::sleep_for(100);
                cap_r.write(true);
            }
            if (get_analog_values().capvolt >= 31)
            { // make me a constant
                // charging complete
                state = FC_RUN;
                motor_r.write(true);
            }
        }
        else if (FC_RUN == state)
        {

            if (purgeTimer.read() > 3 * 60)
            {
                purgeTimer.stop();
                purgeTimer.reset();
                state = FC_PURGE;
                purge_v.write(true);
                purgeTimer.start();
            }

            if (blue_button)
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
        else if (FC_PURGE == state)
        {
            if (purgeTimer.read() > 0.2)
            {
                purge_v.write(false);
                purgeTimer.stop();
                purgeTimer.reset();
            }
            if ((!purge_v)&(get_analog_values().press1>PRESSURE_MIN))
            {
                purgeTimer.start();
                purgeCount = purgeCount + 1;
                state = FC_RUN;
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
