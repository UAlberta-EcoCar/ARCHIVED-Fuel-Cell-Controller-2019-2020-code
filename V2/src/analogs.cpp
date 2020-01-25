#include "mbed.h"

#include "analogs.h"

#include "pin_defs.h"

DigitalOut led2(LED2); // todo - remove

// todo - other sensors
AnalogIn fcvolt(FCVOLT);
AnalogIn motorvolt(MOTORVOLT);
AnalogIn capvolt(CAPVOLT);
AnalogIn fccurr(FCCURR);
AnalogIn motorcurr(MOTORCURR);
AnalogIn capcurr(CAPCURR);

// Possible options for storing data:
    // Map: having key: value pairs would make datalogging a lot easier. Lookup is slow.
    // array: Keeping track of indexes is a pain.
    // struct: pain to type but fast.
analog_values_struct analog_values;

analog_values_struct get_analog_values() {
    // public read-only access to analog_values
    return analog_values;
}

void analog_read_thread() {
    // Loop read adcs at 1kHz (does this actually run at 1 kHz?)
    while (true) {
        // todo - other sensors and scaling
        analog_values.fcvolt = fcvolt.read() * 1.0f + 0.0f;
        analog_values.motorvolt = motorvolt.read();
        analog_values.capvolt = capvolt.read();

        led2 = !led2; // todo - remove
        ThisThread::sleep_for(0.001);
    }
}
