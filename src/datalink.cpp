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

#define FTDI_PER_MS 2000
#define BLUE_PER_MS 2000
#define OL_PER_MS 2000

#define FTDI_BAUD 115200
#define BLUE_BAUD 115200
#define OL_BAUD 115200

#define START_DELAY 2000

EventQueue data_queue(32*EVENTS_EVENT_SIZE);

// Serial printer is gross.
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

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_FCSTATUS
    fdti_printer.print<FuelCell>(&fc, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_FANS
    fdti_printer.print<Fan>(&fan_vec, &fan_iter);
    #endif

    Thread::yield();

    #ifdef ENABLE_SHT31
    fdti_printer.print<SHT31>(&sht31, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_SENSORS
    fdti_printer.print<Sensor>(&sensor_vec, &sensor_iter);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_INTG_VALUES
    fdti_printer.print<Integrator>(&int_vec, &int_iter);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_VANDR
    fdti_printer.print<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter);
    #endif

    fdti_printer.print<string>("\n");
}

void blue_logging(){
    // Temporary
    #ifdef ENABLE_RTC
    blue_printer.print<RealTimeClock>(&rtc, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_FCSTATUS
    blue_printer.print<FuelCell>(&fc, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_FANS
    blue_printer.print<Fan>(&fan_vec, &fan_iter);
    #endif

    Thread::yield();

    #ifdef ENABLE_SHT31
    blue_printer.print<SHT31>(&sht31, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_SENSORS
    blue_printer.print<Sensor>(&sensor_vec, &sensor_iter);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_INTG_VALUES
    blue_printer.print<Integrator>(&int_vec, &int_iter);
    #endif

    Thread::yield();

    #ifdef ENABLE_BLUETOOTH_VANDR
    blue_printer.print<DigitalOut_Ext>(&dig_out_vec, &dig_out_iter);
    #endif

    Thread::yield();

    blue_printer.print<string>("\n");

    
}

void ol_logging(){
    #ifdef ENABLE_RTC
    ol_printer.print<RealTimeClock>(&rtc, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_OPENLOG_FCSTATUS
    ol_printer.print_info<FuelCell>(&fc, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_OPENLOG_FANS
    ol_printer.print_info<Fan>(&fan_vec, &fan_iter, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_OPENLOG_SENSORS
    ol_printer.print_info<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    Thread::yield();

    #ifdef ENABLE_OPENLOG_INTG_VALUES
    ol_printer.print_info<Integrator>(&int_vec, &int_iter);
    #endif
}

void ol_logging_header(){
    #ifdef ENABLE_RTC
    ol_printer.print<string>("Time", 0);
    #endif

    #ifdef ENABLE_OPENLOG_FCSTATUS
    ol_printer.print_header<FuelCell>(&fc, 0);
    #endif

    #ifdef ENABLE_OPENLOG_FANS
    ol_printer.print_header<Fan>(&fan_vec, &fan_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_SENSORS
    ol_printer.print_header<Sensor>(&sensor_vec, &sensor_iter, 0);
    #endif

    #ifdef ENABLE_OPENLOG_INTG_VALUES
    ol_printer.print_header<Integrator>(&int_vec, &int_iter);
    #endif

    Thread::yield();
}

void datalink_thread(){
    // Wait a few seconds for everything to start then start logging
    //Populate vectors
    sensor_vec.push_back(&fccurr);
    sensor_vec.push_back(&capvolt);
    sensor_vec.push_back(&fcvolt);
    sensor_vec.push_back(&capcurr);
    sensor_vec.push_back(&motorvolt);
    sensor_vec.push_back(&motorcurr);
    sensor_vec.push_back(&press1);

    #ifdef ENABLE_PRESS2
    sensor_vec.push_back(&press2);
    #endif
    #ifdef ENABLE_PRESS3
    sensor_vec.push_back(&press3);
    #endif
    #ifdef ENABLE_PRESS4
    sensor_vec.push_back(&press4);
    #endif
  
    sensor_vec.push_back(&fctemp1);
  
    #ifdef ALICE_CONFIGURATION
    sensor_vec.push_back(&fctemp2);
    #endif

    #ifdef ENABLE_TEMP1 
    sensor_vec.push_back(&temp1);
    #endif
    #ifdef ENABLE_TEMP2
    sensor_vec.push_back(&temp2);
    #endif
    #ifdef ENABLE_TEMP3
    sensor_vec.push_back(&temp3);
    #endif
    #ifdef ENABLE_TEMP4
    sensor_vec.push_back(&temp4);
    #endif
    #ifdef ENABLE_TEMP5
    sensor_vec.push_back(&temp5);
    #endif


    int_vec.push_back(&fc_coulumbs);
    int_vec.push_back(&fc_joules);
    int_vec.push_back(&cap_coulumbs);
    int_vec.push_back(&cap_joules);

    dig_out_vec.push_back(&supply_v);
    dig_out_vec.push_back(&purge_v);
    dig_out_vec.push_back(&start_r);
    dig_out_vec.push_back(&motor_r);
    dig_out_vec.push_back(&charge_r);
    dig_out_vec.push_back(&cap_r);
    dig_out_vec.push_back(&fcc_r);

    fan_vec.push_back(&fan1);


    #ifdef ALICE_CONFIGURATION
    #ifdef ENABLE_FAN2
    fan_vec.push_back(&fan2);
    #endif

    #ifdef ENABLE_FAN3
    fan_vec.push_back(&fan3);
    #endif
    #endif

    fc_coulumbs.sensor_add(&fccurr);
    fc_joules.sensor_add(&fccurr);
    fc_joules.sensor_add(&fcvolt);
    cap_coulumbs.sensor_add(&capcurr);
    cap_joules.sensor_add(&capcurr);
    cap_joules.sensor_add(&capvolt);

    blue_logging_event.period(BLUE_PER_MS);
    ol_logging_event.period(OL_PER_MS);
    //ftdi_logging_event.period(FTDI_PER_MS);

    Thread::wait(START_DELAY);

    #ifdef ENABLE_OPENLOG
    #ifdef ENABLE_OPENLOG_HEADER
    ol_logging_header_event.post();
    #endif
    ol_logging_event.post();
    #endif 
    #ifdef ENABLE_BLUETOOTH
    blue_logging_event.post();
    #endif
    //ftdi_logging_event.post();
    
    data_queue.dispatch_forever();
}
