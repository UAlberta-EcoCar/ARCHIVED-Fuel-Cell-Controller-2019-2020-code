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

#define BLUE_PER_MS 2000
#define OL_PER_MS 500

EventQueue data_queue(32*EVENTS_EVENT_SIZE);

SerialPrinter blue_printer("Bluetooth", BLUE_TX, BLUE_RX, 1000000);
SerialPrinter ol_printer("Openlog", OL_TX, OL_RX, 115200);

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
    blue_printer.print<Sensor>(&sensor_vec, &sensor_iter, 0);
    blue_printer.print<Integrator>(&int_vec, &int_iter, 0);
    blue_printer.print<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    blue_printer.print<Fan>(&fan_vec, &fan_iter, 0);
    blue_printer.print<FuelCell>(&fc);
    blue_printer.print<string>("\n", 0);
}

void ol_logging(){
    ol_printer.print_info<Sensor>(&sensor_vec, &sensor_iter, 0);
    ol_printer.print_info<Integrator>(&int_vec, &int_iter, 0);
    ol_printer.print_info<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    ol_printer.print_info<Fan>(&fan_vec, &fan_iter, 0);
    ol_printer.print_info<FuelCell>(&fc);
}

void ol_logging_header(){
    ol_printer.print_name<Sensor>(&sensor_vec, &sensor_iter, 0);
    ol_printer.print_name<Integrator>(&int_vec, &int_iter, 0);
    ol_printer.print_name<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    ol_printer.print<string>("FC_Status Error_Status");
}

void datalink_thread(){
    blue_logging_event.period(BLUE_PER_MS);
    ol_logging_event.period(OL_PER_MS);

    ol_logging_header_event.post();
    ol_logging_event.post();
    blue_logging_event.post();

    data_queue.dispatch_forever();
}
