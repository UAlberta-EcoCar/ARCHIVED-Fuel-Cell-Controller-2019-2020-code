#include <mbed.h>
#include <mbed_events.h>
#include <string>

// Classes
#include "Classes/SerialPrinter.h"


// Defs
#include "Def/pin_def.h"
#include "Def/object_def.h"

#include "monitoring.h"
#include "main.h"

#define FTDI_PER_MS 1000
#define BLUE_PER_MS 5000
#define OL_PER_MS 500

#define FTDI_BAUD 1000000
#define BLUE_BAUD 9600
#define OL_BAUD 115200

EventQueue data_queue(32*EVENTS_EVENT_SIZE);

SerialPrinter blue_printer("Bluetooth", BLUE_TX, BLUE_RX, BLUE_BAUD);
SerialPrinter ol_printer("Openlog", OL_TX, OL_RX, OL_BAUD);
SerialPrinter fdti_printer("FTDI", FTDI_TX, FTDI_RX, FTDI_BAUD);

void error_logging();
void ol_logging();
void ol_logging_header();
void blue_logging();
void ftdi_logging();

Event<void()> blue_logging_event(&data_queue, blue_logging);
Event<void()> ftdi_logging_event(&data_queue, ftdi_logging);
Event<void()> ol_logging_event(&data_queue, ol_logging);
Event<void()> ol_logging_header_event(&data_queue, ol_logging_header);

void error_logging(){
    ol_printer.print<string>("\nWARNING AN ERROR HAS OCCURED SHUTTING DOWN: ", 0);
    ol_printer.print<int>(fc.get_error_status());
    blue_printer.print<string>("\nWARNING AN ERROR HAS OCCURED SHUTTING DOWN: ", 0);
    blue_printer.print<int>(fc.get_error_status());
}

void ftdi_logging(){
    #ifdef ENABLE_RTC
    fdti_printer.print<RealTimeClock>(&rtc, 0);
    #endif

    #ifdef ENABLE_SHT31
    fdti_printer.print<SHT31>(&sht31, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_SENSORS
    fdti_printer.print<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_INTG_VALUES
    fdti_printer.print<Integrator>(&int_vec, &int_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_VANDR
    fdti_printer.print<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_FANS
    fdti_printer.print<Fan>(&fan_vec, &fan_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_FCSTATUS
    fdti_printer.print<FuelCell>(&fc);
    #endif

    fdti_printer.print<int>(controller_flags.get());


}

void blue_logging(){
    // Temporary
    #ifdef ENABLE_RTC
    blue_printer.print<RealTimeClock>(&rtc, 0);
    #endif

    #ifdef ENABLE_SHT31
    blue_printer.print<SHT31>(&sht31, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_SENSORS
    blue_printer.print<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_INTG_VALUES
    blue_printer.print<Integrator>(&int_vec, &int_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_VANDR
    blue_printer.print<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_FANS
    blue_printer.print<Fan>(&fan_vec, &fan_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_FCSTATUS
    blue_printer.print<FuelCell>(&fc);
    #endif

    blue_printer.print<int>(controller_flags.get());
}

void ol_logging(){
    #ifdef ENABLE_OPENLOG_SENSORS
    ol_printer.print_info<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_INTG_VALUES
    ol_printer.print_info<Integrator>(&int_vec, &int_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_VANDR
    ol_printer.print_info<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_FANS
    ol_printer.print_info<Fan>(&fan_vec, &fan_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_FCSTATUS
    ol_printer.print_info<FuelCell>(&fc, 0);
    #endif

    ol_printer.print<string>("\n", 0);

}

void ol_logging_header(){
    #ifdef ENABLE_OPENLOG_SENSORS
    ol_printer.print_header<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_INTG_VALUES
    ol_printer.print_header<Integrator>(&int_vec, &int_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_VANDR
    ol_printer.print_header<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_FANS
    ol_printer.print_header<Fan>(&fan_vec, &fan_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_FCSTATUS
    ol_printer.print<string>("FC_Status Error_Status", 0);
    #endif

    ol_printer.print<string>("\n", 0);
}

void datalink_thread(){
    // Wait a few seconds for everything to start then start logging
    Thread::wait(10000);

    blue_logging_event.period(BLUE_PER_MS);
    ol_logging_event.period(OL_PER_MS);
    ftdi_logging_event.period(FTDI_PER_MS);

    #ifdef ENABLE_OPENLOG_HEADER
    ol_logging_header_event.post();
    #endif

    #ifdef ENABLE_OPENLOG
    ol_logging_event.post();
    #endif 

    #ifdef ENABLE_BLUETOOTH
    blue_logging_event.post();
    #endif

    ftdi_logging_event.post();
    
    data_queue.dispatch_forever();
}
