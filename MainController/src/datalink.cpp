#include <mbed.h>
#include <mbed_events.h>
#include <string>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"
#include "Classes/SerialPrinter.h"


// Defs
#include "Def/pin_def.h"
#include "Def/object_def.h"

#include "monitoring.h"
#include "main.h"

#define BLUE_PER_MS 2000
#define OL_PER_MS 500

#define FTDI_BAUD 1000000
#define BLUE_BAUD 1000000
#define OL_BAUD 115200

EventQueue data_queue(32*EVENTS_EVENT_SIZE);

SerialPrinter blue_printer("Bluetooth", FTDI_TX, FTDI_RX, BLUE_BAUD);
SerialPrinter ol_printer("Openlog", OL_TX, OL_RX, OL_BAUD);

void error_logging();
void ol_logging();
void ol_logging_header();
void blue_logging();

Event<void()> blue_logging_event(&data_queue, blue_logging);
Event<void()> ol_logging_event(&data_queue, ol_logging);
Event<void()> ol_logging_header_event(&data_queue, ol_logging_header);

void error_logging(){
    ol_printer.print<string>("\nWARNING AN ERROR HAS OCCURED SHUTTING DOWN: ", 0);
    ol_printer.print<int>(fc.get_error_status());
    blue_printer.print<string>("\nWARNING AN ERROR HAS OCCURED SHUTTING DOWN: ", 0);
    blue_printer.print<int>(fc.get_error_status());
}

void blue_logging(){
    blue_printer.print<string>(rtc.toString(), 0);
    #ifdef ENABLE_BLUETOOTH_SENSORS
    blue_printer.print<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    #ifdef ENABLE_BLUETOOTH_INTG
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

    blue_printer.print<string>("\n", 0);
    
}

void ol_logging(){
    #ifdef ENABLE_OPENLOG_SENSORS
    ol_printer.print_info<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_INTG
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
    ol_printer.print_name<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_INTG
    ol_printer.print_name<Integrator>(&int_vec, &int_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_VANDR
    ol_printer.print_name<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_FANS
    ol_printer.print_name<Fan>(&fan_vec, &fan_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_FCSTATUS
    ol_printer.print<string>("FC_Status Error_Status", 0);
    #endif

    ol_printer.print<string>("\n", 0);
}

void datalink_thread(){
    blue_logging_event.period(BLUE_PER_MS);
    ol_logging_event.period(OL_PER_MS);

    #ifdef ENABLE_OPENLOG_HEADER
    ol_logging_header_event.post();
    #endif

    #ifdef ENABLE_OPENLOG
    ol_logging_event.post();
    #endif 

    #ifdef ENABLE_BLUETOOTH
    blue_logging_event.post();
    #endif
    // Wait seconds for everything to start then start logging
    Thread::wait(2000);
    data_queue.dispatch_forever();
}
