/*
 *
 *  @Company
        Kettering University
 *
 *  @File Name
 *      accelerometer.c
 *
 *  @Summary
 *      Driver for the MMA8451 accelerometer
 *
 *  @Description
 *      Reads the MMA8451 accelerometer over the software I2C bus. See
 *      example16() in examples.c for a demo displaying the values on the OLED.
 *
 *  @Authors:
 *      Osfield Gaga
 *
 *  @Created on
 *      October 28, 2026
 *
 *  @Last modification
 *      October 28, 2026
 */

#include "xc.h"
#include <stdint.h>

#include "i2c_master.h"
#include "accelerometer.h"

// Write a single register
void accel_write_reg(uint8_t reg, uint8_t data) {
    I2CM_Start();
    I2CM_Write(MMA8451_ADDR << 1); // address + write bit
    I2CM_Write(reg); // register
    I2CM_Write(data); // data
    I2CM_Stop();
}

// Burst read len bytes, starting at reg. The MMA8451 auto-increments the
// register address after each byte, and requires a repeated start (no stop)
// between the register write and the read
void accel_read_regs(uint8_t reg, uint8_t *buf, uint8_t len) {
    I2CM_Start();
    I2CM_Write(MMA8451_ADDR << 1); // address + write bit
    I2CM_Write(reg); // register

    I2CM_Start(); // repeated start
    I2CM_Write((MMA8451_ADDR << 1) | 0x01); // address + read bit

    for (uint8_t i = 0; i < len; i++) {
        // ACK every byte except the last one, which gets a NACK
        buf[i] = I2CM_Read(i == len - 1);
    }

    I2CM_Stop();
}

uint8_t init_accelerometer(void) {
    uint8_t id;

    I2CM_Init();

    accel_read_regs(MMA8451_WHO_AM_I, &id, 1);
    if (id != MMA8451_DEVICE_ID) {
        return 0;
    }

    // the configuration registers can only be changed in standby mode
    accel_write_reg(MMA8451_CTRL_REG1, 0x00);

    // 0x00 = +/-2g full scale range
    accel_write_reg(MMA8451_XYZ_DATA_CFG, 0x00);

    // 0x02 = high resolution oversampling mode
    accel_write_reg(MMA8451_CTRL_REG2, 0x02);

    // 0x05 = active mode + low noise, 800 Hz data rate
    accel_write_reg(MMA8451_CTRL_REG1, 0x05);

    return 1;
}

void accel_read(accel_data_t *data) {
    uint8_t buf[6];

    accel_read_regs(MMA8451_OUT_X_MSB, buf, 6);

    // each axis is a 14 bits value, MSB first and left aligned in 16 bits,
    // the shift keeps the sign
    data->x = ((int16_t) ((buf[0] << 8) | buf[1])) >> 2;
    data->y = ((int16_t) ((buf[2] << 8) | buf[3])) >> 2;
    data->z = ((int16_t) ((buf[4] << 8) | buf[5])) >> 2;
}

int16_t accel_to_mg(int16_t raw) {
    // cast to 32 bits to avoid overflowing during the multiplication
    return (int16_t) (((int32_t) raw * 1000) / MMA8451_COUNTS_PER_G);
}
