#include "SHT31.h"

SHT31::SHT31(PinName sda, PinName scl):master(sda,scl)
{
    for (int i = 0; i < 6; i++){
        this->slave_buffer[i] = 0;
    }
}

void SHT31::send_command(uint16_t command){
    char cmd[CMDSIZE];
    for (int i = 0; i < CMDSIZE; i++){
        cmd[i] = command&0xff;
        command = command >> 8;
    }
    this->master.lock();
    this->master.write(SHT31_DEFAULT_ADDR, cmd, CMDSIZE);
    this->master.unlock();
}

void SHT31::read_slave(char* buffer){
    this->master.lock();
    this->master.read(SHT31_DEFAULT_ADDR, buffer, BUFFERSIZE);
    this->master.unlock();
}

void SHT31::lock(){
    this->master.lock();
}

void SHT31::unlock(){
    this->master.unlock();
}