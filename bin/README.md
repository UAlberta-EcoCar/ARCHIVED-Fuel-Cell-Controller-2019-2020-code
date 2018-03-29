# Bin
This directory contains precompiled binaries, as compiling mbedOS in my experiance takes 6-10 minutes on a laptop (Desktop ~30 seconds).

## Competion Binary
Binary for raceday
* Disables OLED Screen
* Disables SHT31
* Disables FTDI logging

## Testing Binary's
### Test Bench Binary(s)
Intended for use on the test-bench
* Disables external start button


### State Machine Sequence Test
Intended for testing the state machines sequence of events.
* Disables state checks
* Disables error's

### State Machine Transistion Test
Intended for testing that the state machine responds to transistion conditions appropriatley
* Disables error's

### Sensor Calibration 
Intended for when calibrating sensor's
* Disables OpenLog
* Disables SHT31
* Disables State Machine
* Increases Bluetooth/FTDI logging frequency