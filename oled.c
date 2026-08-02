/*
 * 
 *  @Company
        Kettering University
 * 
 *  @File Name
 *      oled.c
 * 
 *  @Summary
 *      This file sets up the OLED display and provides functions to interact with it 
 * 
 *  @Description
 *      The keypad.c file contains the main initialization and configuration of 
 *      the LEDs. It provides functions to configure the IO pins and read the
 *      input keys. The functions can either affect all LEDs at once, or manipulate 
 *      them individually.
 * 
 *  @Authors: 
 *      Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 28, 2025, 7:52 PM
 * 
 *  @Last modification
 *      December 09, 2025, 12:17 PM
 */
#define FCY 4000000UL
#include "xc.h"
#include "i2c_master.h"
#include "stdint.h"
#include <libpic30.h>
#include <math.h>
#include "oled.h"

void init_oled(void) {
    
    ANSELBbits.ANSELB8 = 0;
    ANSELBbits.ANSELB9 = 0;
    
    I2CM_Init();
    
    I2CM_Start();
    
    I2CM_Write(0x3C << 1);
    I2CM_Write(0x00);
    I2CM_Write(0xAE); // Display Off
    I2CM_Write(0xD5); I2CM_Write(0x80); // Clock
    I2CM_Write(0xA8); I2CM_Write(0x3F); // Multiplex ratio ? fix from 0x00
    I2CM_Write(0xD3); I2CM_Write(0x00); // Display offset
    I2CM_Write(0x40);                   // Start line
    I2CM_Write(0x8D); I2CM_Write(0x14); // Charge pump on
    I2CM_Write(0x20); I2CM_Write(0x00); // Horizontal addressing ? add this
    I2CM_Write(0xA1);                   // Segment remap
    I2CM_Write(0xC8);                   // COM scan direction
    I2CM_Write(0xDA); I2CM_Write(0x12); // COM pins
    I2CM_Write(0x81); I2CM_Write(0xFF); // Contrast
    I2CM_Write(0xD9); I2CM_Write(0x22); // Pre-charge
    I2CM_Write(0xDB); I2CM_Write(0x30); // VCOMH
    I2CM_Write(0xA4);                   // Output follows RAM
    I2CM_Write(0xA6);                   // Normal display
    I2CM_Write(0xAF); // Display On
    
    I2CM_Stop();
}

// Send a command
void oled_cmd(uint8_t cmd) {
    I2CM_Start();
    I2CM_Write(0x3C << 1);  // address + write bit
    I2CM_Write(0x00);       // control byte: command
    I2CM_Write(cmd);
    I2CM_Stop();
}

// Send data bytes (e.g. frame buffer)
void oled_data(uint8_t *buf, uint16_t len) {
    I2CM_Start();
    I2CM_Write(0x3C << 1);  // address + write bit
    I2CM_Write(0x40);       // control byte: data
    for (uint16_t i = 0; i < len; i++) {
        I2CM_Write(buf[i]);
    }
    I2CM_Stop();
}

void oled_fill(uint8_t pattern) {
    // Set address window
    I2CM_Start();
    I2CM_Write(0x3C << 1);
    I2CM_Write(0x00);        // command stream
    I2CM_Write(0x21);        // column address
    I2CM_Write(0x00);        // start 0
    I2CM_Write(0x7F);        // end 127
    I2CM_Write(0x22);        // page address
    I2CM_Write(0x00);        // start 0
    I2CM_Write(0x07);        // end 7
    I2CM_Stop();

    // Small delay between transactions
    __delay_ms(10);

    // Send all 1024 pixels
    I2CM_Start();
    I2CM_Write(0x3C << 1);
    I2CM_Write(0x40);        // data stream
    for (uint16_t i = 0; i < 1024; i++) {
        I2CM_Write(pattern);
    }
    I2CM_Stop();
}

// framebuffer - lives in RAM on the MCU


// set or clear a single pixel in the framebuffer
void oled_set_pixel(uint8_t x, uint8_t y, uint8_t on) {
    if (x >= 128 || y >= 64) return;  // bounds check
    uint8_t page = y / 8;
    uint8_t bit  = y % 8;
    if (on)
        fb[page * 128 + x] |=  (1 << bit);
    else
        fb[page * 128 + x] &= ~(1 << bit);
}

// push framebuffer to display
void oled_update(void) {
    I2CM_Start();
    I2CM_Write(0x3C << 1);
    I2CM_Write(0x00);
    I2CM_Write(0x21); I2CM_Write(0x00); I2CM_Write(0x7F);
    I2CM_Write(0x22); I2CM_Write(0x00); I2CM_Write(0x07);
    I2CM_Stop();

    I2CM_Start();
    I2CM_Write(0x3C << 1);
    I2CM_Write(0x40);
    for (uint16_t i = 0; i < 1024; i++)
        I2CM_Write(fb[i]);
    I2CM_Stop();
}

// draw a line from (x0,y0) to (x1,y1) using Bresenham's algorithm
void oled_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t on) {
    int dx =  abs((int)x1 - (int)x0);
    int dy = -abs((int)y1 - (int)y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        oled_set_pixel(x0, y0, on);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}



void oled_draw_char(uint8_t x, uint8_t page, char c) {
    if (c < 32 || c > 126) return;
    const uint8_t *glyph = font5x8[c - 32];
    for (uint8_t col = 0; col < 5; col++) {
        uint8_t bits = glyph[col];
        for (uint8_t row = 0; row < 8; row++) {
            oled_set_pixel(x + col, page * 8 + row, (bits >> row) & 1);
        }
    }
    // one column gap after character
    for (uint8_t row = 0; row < 8; row++)
        oled_set_pixel(x + 5, page * 8 + row, 0);
}

void oled_clear_line(uint8_t page) {
    uint8_t x = 0;
    while (x < 128) {
        oled_draw_char(x, page, 32); // 32 is space or just nothing
        x += 6;  // 5 pixels wide + 1 gap
    }
}

void oled_draw_string(uint8_t page, const char *str) {
    // clear line before writing to prevent ghosts
    oled_clear_line(page);
    
    uint8_t x = 0;
    while (*str && x < 128) {
        oled_draw_char(x, page, *str++);
        x += 6;  // 5 pixels wide + 1 gap
    }
}



void oled_draw_bitmap(const uint8_t *bmp) {
    memcpy(fb, bmp, 1024);
    oled_update();
}