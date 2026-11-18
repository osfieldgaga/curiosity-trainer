/* **************************************************************************
*
*  @Company
*    Kettering University
*
*  @File Name
*    accelerometer.h
*
*  @Summary
*    Driver for the MMA8451 3-axis accelerometer over I2C
*
*  @Description
*    The MMA8451 (Adafruit product 2019) shares the software I2C bus
*    (i2c_master.c/h) with the OLED and the IO expander. On the Adafruit
*    breakout the sensor answers at address 0x1D by default, use 0x1C if the
*    A pin is tied to GND.
*
*    The accelerometer is configured for a +/-2g range with 14 bits
*    resolution, which gives 4096 counts per g. Refer to the "MMA8451Q"
*    datasheet from NXP for a complete overview of the registers.
*
*  @Author
*    Osfield Gaga
*
*  @Created on
*      October 28, 2026
*
*  @Last modification
*      October 28, 2026
************************************************************************** */

#ifndef ACCELEROMETER_H
#define	ACCELEROMETER_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

/******************************************************************************
 *                          MMA8451 configuration
 *
 ******************************************************************************/

#define MMA8451_ADDR 0x1D // 0x1C if the A pin is tied to GND

// Registers used by the driver
#define MMA8451_OUT_X_MSB    0x01 // first of 6 bytes: X, Y, Z (MSB first)
#define MMA8451_WHO_AM_I     0x0D // reads 0x1A on a MMA8451
#define MMA8451_XYZ_DATA_CFG 0x0E // full scale range
#define MMA8451_CTRL_REG1    0x2A // active/standby, data rate
#define MMA8451_CTRL_REG2    0x2B // oversampling mode

#define MMA8451_DEVICE_ID 0x1A
#define MMA8451_COUNTS_PER_G 4096 // at +/-2g range, 14 bits

/******************************************************************************
 *                              Functions
 *
 ******************************************************************************/

// raw 14 bits accelerometer readings, 4096 = 1g
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} accel_data_t;

uint8_t init_accelerometer(void); // configures the sensor, returns 1 if found, 0 otherwise
void accel_write_reg(uint8_t reg, uint8_t data); // write a single register
void accel_read_regs(uint8_t reg, uint8_t *buf, uint8_t len); // burst read starting at reg
void accel_read(accel_data_t *data); // read raw X, Y, Z
int16_t accel_to_mg(int16_t raw); // convert a raw reading to milli-g

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ACCELEROMETER_H */
