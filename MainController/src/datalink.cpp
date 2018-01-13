#include <mbed.h>
#include <mbed_events.h>
#include <string>

// Classes
#include "Classes/Analog_Sensor.h"
#include "Classes/Fan.h"
#include "Classes/DigitalOut_Ext.h"
#include "Classes/Integrator.h"
#include "Classes/LinearScalable.h"
#include "Classes/SerialPrinter.h"

// Defs
#include "Def/constants.h"
#include "Def/pin_def.h"
#include "Def/object_def.h"
#include "Def/thread_def.h"
#include "Def/semaphore_def.h"

// Thread src
#include "controller_event_queue.h"
#include "monitoring.h"
#include "error_event_queue.h"
#include "main.h"

#include "fc_status.h"

SerialPrinter blue_printer("Bluetooth", BLUE_TX, BLUE_RX, 1000000);
SerialPrinter ol_printer("Openlog", OL_TX, OL_RX, 1000000);
Ticker update_ol;
Ticker update_blue;
EventQueue data_queue(32*EVENTS_EVENT_SIZE);
Semaphore stall(0);

void error_logging(){
    ol_printer.print<string>("\nWARNING AN ERROR HAS OCCURED SHUTTING DOWN: ", 0);
    Thread::wait(1);
    ol_printer.print<string>("error");
    Thread::wait(1);
    blue_printer.print<string>("\nWARNING AN ERROR HAS OCCURED SHUTTING DOWN: ", 0);
    Thread::wait(1);
    blue_printer.print<string>("error");
}

void blue_logging(){
    blue_printer.print<Sensor>(&sensor_vec, &sensor_iter, 0);
    Thread::wait(1);
    blue_printer.print<Integrator>(&int_vec, &int_iter, 0);
    Thread::wait(1);
    blue_printer.print<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    Thread::wait(1);
    blue_printer.print<string>("FC Status:", 0);
    Thread::wait(1);
    blue_printer.print<int>(get_fc_status());
    Thread::wait(1);
}

void ol_logging(){
    ol_printer.print_info<Sensor>(&sensor_vec, &sensor_iter, 0);
    Thread::wait(1);
    ol_printer.print_info<Integrator>(&int_vec, &int_iter, 0);
    Thread::wait(1);
    ol_printer.print_info<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    Thread::wait(1);
    ol_printer.print<int>(get_fc_status());
    Thread::wait(1);
}

void datalink_thread(){
    //TODO: Printer time via rtc
    // Print name headers for OL
    ol_printer.print_name<Sensor>(&sensor_vec, &sensor_iter, 0);
    ol_printer.print_name<Integrator>(&int_vec, &int_iter, 0);
    ol_printer.print_name<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter, 0);
    ol_printer.print<string>("FC Status");

    // Start periodic logging
    update_ol.attach(&ol_logging, 0.5);
    update_blue.attach(&blue_logging, 2);
    data_queue.dispatch();
    stall.wait();
}
