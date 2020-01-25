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
AnalogIn press1(PRESS1);
AnalogIn fctemp1(FCTEMP1);

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
        analog_values.fcvolt = fcvolt.read() * 26.70162f - 0.0357f;
        analog_values.motorvolt = motorvolt.read() * 26.70162f - 0.0357f;
        analog_values.capvolt = capvolt.read() * 26.70162f - 0.0357f;
        analog_values.fccurr = fccurr.read() * 157.7268f - 58.099f;
        analog_values.motorcurr = motorcurr.read() * 315.4536f - 199.4556f;
        analog_values.capcurr = capcurr.read() * 157.7268f - 58.099f;
        analog_values.press1 = press1.read() * 61.8057f - 23.353f;
        float x = fctemp1.read();
        analog_values.fctemp1 = pow(x, 5)*140.0978f + pow(x, 4)*-899.05152f + pow(x, 3)*3595.492f +
        pow(x, 2)*-7539.7506f + pow(x, 1)* 7471.2785f - 2818.1076f;



        led2 = !led2; // todo - remove
        ThisThread::sleep_for(0.001);
    }
}
