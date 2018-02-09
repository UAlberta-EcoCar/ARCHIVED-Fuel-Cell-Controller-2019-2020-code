#ifndef SHT31_H
#define SHT31_H
#include <mbed.h>

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

#define BUFFERSIZE 6
#define CMDSIZE 2

class SHT31 {
    private:
        I2C master;
        char slave_buffer[BUFFERSIZE];


    public:

        SHT31(PinName sda, PinName scl);

        void send_command(uint16_t command);
        void read_slave(char* buffer);
        void lock();
        void unlock();
};

#endif