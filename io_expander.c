/*
 * File:   io_expander.c
 * Author: Osfield
 *
 * Created on April 15, 2026, 11:16 AM
 */


#include "xc.h"
#include "i2c_master.h"

// Send a command
void io_cmd(uint8_t reg, uint8_t data) {
    I2CM_Start();
    I2CM_Write(0x20 << 1);  // address + write bit
    I2CM_Write(reg);       // register
    I2CM_Write(data);       // data
    I2CM_Stop();
}

void init_IO_expander(void) {
    I2CM_Init();
    
    //0x03 = config register to set input of output, 
    //0xFF = all pins set to input
    io_cmd(0x03, 0xFF) ;
    
}

uint8_t io_read() {
    uint8_t data;
    
    I2CM_Start();
    I2CM_Write(0x20 << 1);  // address + write bit
    I2CM_Write(0x00);       // register
    
    I2CM_Start(); // start again
    I2CM_Write((0x20 << 1) | 0x01);  // address + write bit
    
    data = I2CM_Read(1); // send NACK
    
    I2CM_Stop();
    
    return data;
}



void pinMode(uint8_t pin, uint8_t direction) {
    
}
