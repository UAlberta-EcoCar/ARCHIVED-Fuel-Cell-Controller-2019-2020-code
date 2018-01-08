#include <mbed.h>
#include <mbed_events.h>

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
#include "controller_states.h"
#include "monitoring.h"
#include "error_event_queue.h"
#include "main.h"

#include "fc_status.h"

SerialPrinter blue_printer("Bluetooth", BLUE_TX, BLUE_RX, 1000000);
SerialPrinter ol_printer("Openlog", OL_TX, OL_RX, 1000000);
Ticker update;
EventQueue data_queue(32*EVENTS_EVENT_SIZE);
Semaphore stall(0);

void blue_logging(){
    blue_printer.print<Sensor>(&sensor_vec, &sensor_iter, 0);
    blue_printer.print<Integrator>(&int_vec, &int_iter, 0);
    blue_printer.print<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter);
}

void ol_logging(){
    ol_printer.print_info<Sensor>(&sensor_vec, &sensor_iter, 0);
    ol_printer.print_info<Integrator>(&int_vec, &int_iter, 0);
    ol_printer.print_info<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter);
}

void push_datalog(){
    data_queue.call(ol_logging);
    data_queue.call(blue_logging);
}

void datalink_thread(){
    //TODO: Printer time via rtc
    ol_printer.print_names<Sensor>(&sensor_vec, &sensor_iter, 0);
    ol_printer.print_names<Integrator>(&int_vec, &int_iter, 0);
    ol_printer.print_names<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter);
    update.attach(&push_datalog, 0.01);
    data_queue.dispatch();
    stall.wait();
}
