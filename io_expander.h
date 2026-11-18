/* **************************************************************************
*
*  @Company
*    Kettering University
*
*  @File Name
*    io_expander.h
*
*  @Summary
*    Driver for the TCA9534 8-bit I2C IO expander
*
*  @Description
*    The TCA9534 (Texas Instruments) adds 8 IO pins, P0 to P7, on the software
*    I2C bus (i2c_master.c/h) shared with the OLED and the accelerometer. Its
*    address pins A2-A0 are tied low on the board: address 0x20.
*
*    Each pin is an input or a push-pull output (configuration register).
*    The TCA9534 has no internal pull-up resistors: an input pin left
*    unconnected floats and reads a random level. An output pin can sink at
*    most 25 mA. Refer to the "TCA9534" datasheet from Texas Instruments
*    (datasheets folder) for a complete overview of the registers.
*
*  @Author
*    Osfield Gaga
*
*  @Created on
*      April 15, 2026
*
*  @Last modification
*      November 17, 2026
************************************************************************** */

#ifndef IOEXPANDER_H
#define	IOEXPANDER_H

#include <xc.h> // include processor files - each processor file is guarded.

#include <stdint.h>
#include <libpic30.h>

/******************************************************************************
 *                          TCA9534 configuration
 *
 ******************************************************************************/

#define IOEXP_ADDR 0x20 // A2 = A1 = A0 = low

// Registers (command byte)
#define IOEXP_REG_INPUT    0x00 // read only: level on each pin, inputs and outputs
#define IOEXP_REG_OUTPUT   0x01 // level driven on the output pins, 0xFF at power-up
#define IOEXP_REG_POLARITY 0x02 // 1 = the input register bit is inverted, 0x00 at power-up
#define IOEXP_REG_CONFIG   0x03 // 1 = input, 0 = output, 0xFF at power-up (all inputs)

// Pin directions for io_pin_mode()
#define IOEXP_OUTPUT 0
#define IOEXP_INPUT  1

/******************************************************************************
 *                              Functions
 *
 ******************************************************************************/

uint8_t init_IO_expander(void); // all pins inputs, returns 1 if the TCA9534 answered, 0 otherwise

// whole port, bit i = pin Pi
uint8_t io_read(void); // levels on the 8 pins (input register)
void io_write(uint8_t value); // levels driven on the output pins
void io_set_direction(uint8_t inputs); // 1 = input, 0 = output, e.g. 0xF0 = P0-P3 outputs

// single pin, pin = 0 to 7
void io_pin_mode(uint8_t pin, uint8_t direction); // IOEXP_INPUT or IOEXP_OUTPUT
void io_write_pin(uint8_t pin, uint8_t level); // drive an output pin high (1) or low (0)
uint8_t io_read_pin(uint8_t pin); // level on a pin, 0 or 1

// registers
void io_cmd(uint8_t reg, uint8_t data); // write a register
uint8_t io_read_reg(uint8_t reg); // read a register

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* IOEXPANDER_H */
