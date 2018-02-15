#ifndef SHT31_H
#define SHT31_H
#include <mbed.h>
#include "main.h"
#include "IO.h"

#define SHT31_DEFAULT_ADDR         0x44
#define SHT31_MEAS_HIGHREP_STRETCH 0x2C06
#define SHT31_MEAS_MEDREP_STRETCH  0x2C0D
#define SHT31_MEAS_LOWREP_STRETCH  0x2C10
#define SHT31_MEAS_HIGHREP         0x2400
#define SHT31_MEAS_MEDREP          0x240B
#define SHT31_MEAS_LOWREP          0x2416
#define SHT31_READSTATUS           0xF32D
#define SHT31_CLEARSTATUS          0x3041
#define SHT31_SOFTRESET            0x30A2
#define SHT31_HEATEREN             0x306D
#define SHT31_HEATERDIS            0x3066

#define SHT31_BUFFERSIZE 6
#define SHT31_CMDSIZE 2

// For communicating with the SHT31 device
class SHT31: public IO {

private:
    I2C *master;
    struct{
        int humidity;
        int temp;
    }readings;

protected:

    void read_device();

public:

    SHT31(string name, I2C *master);

    int get_temp(bool update=1);
    int get_hum(bool update=1);

    string toString(bool json_format = 0);
    string toStringInfo(bool json_format = 0);
};

#endif