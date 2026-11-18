/*
 *
 *  @Company
        Kettering University
 *
 *  @File Name
 *      io_expander.c
 *
 *  @Summary
 *      Driver for the TCA9534 8-bit I2C IO expander
 *
 *  @Description
 *      Reads and writes the 8 pins of the TCA9534 over the software I2C bus.
 *      The configuration and output registers are kept in RAM (shadow copies),
 *      so changing one pin does not need to read the register first. See
 *      example14() in examples.c for a demo displaying the inputs on the OLED.
 *
 *  @Authors:
 *      Osfield Gaga
 *
 *  @Created on
 *      April 15, 2026
 *
 *  @Last modification
 *      November 17, 2026
 */

#include "xc.h"
#include <stdint.h>

#include "i2c_master.h"
#include "io_expander.h"

// last values written to the configuration and output registers
static uint8_t ioConfig = 0xFF;
static uint8_t ioOutput = 0xFF;

// Write a register
void io_cmd(uint8_t reg, uint8_t data) {
    I2CM_Start();
    I2CM_Write(IOEXP_ADDR << 1); // address + write bit
    I2CM_Write(reg); // command byte: register
    I2CM_Write(data); // data
    I2CM_Stop();

    // keep the shadow copies in sync, also when io_cmd() is called directly
    if (reg == IOEXP_REG_CONFIG) {
        ioConfig = data;
    } else if (reg == IOEXP_REG_OUTPUT) {
        ioOutput = data;
    }
}

// Read a register: the command byte selects the register, then a repeated
// start switches to reading
uint8_t io_read_reg(uint8_t reg) {
    uint8_t data;

    I2CM_Start();
    I2CM_Write(IOEXP_ADDR << 1); // address + write bit
    I2CM_Write(reg); // command byte: register

    I2CM_Start(); // repeated start
    I2CM_Write((IOEXP_ADDR << 1) | 0x01); // address + read bit

    data = I2CM_Read(1); // single byte: NACK

    I2CM_Stop();

    return data;
}

uint8_t init_IO_expander(void) {
    short nack;

    I2CM_Init();

    // check that the TCA9534 answers at its address
    I2CM_Start();
    nack = I2CM_Write(IOEXP_ADDR << 1);
    I2CM_Stop();
    if (nack) {
        return 0;
    }

    // start from the output register in the device (0xFF at power-up), so a
    // pin later switched to output drives the level already in the register
    ioOutput = io_read_reg(IOEXP_REG_OUTPUT);

    io_cmd(IOEXP_REG_POLARITY, 0x00); // inputs not inverted
    io_cmd(IOEXP_REG_CONFIG, 0xFF); // all pins inputs

    return 1;
}

uint8_t io_read(void) {
    return io_read_reg(IOEXP_REG_INPUT);
}

void io_write(uint8_t value) {
    io_cmd(IOEXP_REG_OUTPUT, value);
}

void io_set_direction(uint8_t inputs) {
    io_cmd(IOEXP_REG_CONFIG, inputs);
}

void io_pin_mode(uint8_t pin, uint8_t direction) {
    if (pin > 7) return;

    if (direction == IOEXP_INPUT) {
        io_cmd(IOEXP_REG_CONFIG, ioConfig | (1 << pin));
    } else {
        io_cmd(IOEXP_REG_CONFIG, ioConfig & ~(1 << pin));
    }
}

void io_write_pin(uint8_t pin, uint8_t level) {
    if (pin > 7) return;

    if (level) {
        io_cmd(IOEXP_REG_OUTPUT, ioOutput | (1 << pin));
    } else {
        io_cmd(IOEXP_REG_OUTPUT, ioOutput & ~(1 << pin));
    }
}

uint8_t io_read_pin(uint8_t pin) {
    if (pin > 7) return 0;

    return (io_read() >> pin) & 0x01;
}
