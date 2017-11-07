#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

// Getter
bool get_supply_valve();
bool get_purge_valve();
bool get_start_relay();
bool get_motor_relay();
bool get_charge_relay();
bool get_cap_relay();
bool get_fcc_relay();

// Setter
void set_supply_valve(bool val);
void set_purge_valve(bool val);
void set_start_relay(bool val);
void set_motor_relay(bool val);
void set_charge_relay(bool val);
void set_cap_relay(bool val);
void set_fcc_relay(bool val);

#endif
