#include "mbed.h"

#include "analogs.h"

#include "pin_defs.h"

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
        analog_values.fcvolt = fcvolt.read() * 48.996f - 0.2139f; //updated scaling factor, tested accurate
        analog_values.motorvolt = motorvolt.read() * 26.70162f - 0.0357f; //not working properly
        analog_values.capvolt = capvolt.read() *  48.645f - 0.158f; //updated scaling factor, tested accurate
        analog_values.fccurr = 0.6494*(fccurr.read() * 265.369f - 97.9556f) + 0.3799; //updated scaling factors, tested within an amp
        analog_values.motorcurr = motorcurr.read() * 133.083f - 50.5399f; //updated scaling factors, tested within an amp
        analog_values.capcurr = capcurr.read() * 157.7268f - 58.099f; //not working properly
        analog_values.press1 = press1.read() * 61.8057f - 23.353f;
        float x = fctemp1.read();
        analog_values.fctemp1 = pow(x, 5)*(-2818.1076f) + pow(x, 4)*(7471.2785f) + pow(x, 3)*(-7539.7506f) +
        pow(x, 2)*(3595.492f) + pow(x, 1)*(-899.05152f)  + 140.0978f;

        ThisThread::sleep_for(0.001);
    }
}
