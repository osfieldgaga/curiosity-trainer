/*
 * File:   neopixels.c
 * Author: Osfield
 *
 * Created on December 17, 2025, 1:34 PM
 */

#include <stdint.h>
#include "xc.h"
#include "neopixels.h"
#include "config.h"
#include <libpic30.h>

#include "sk6812.h"


void NEO_Init() {
    NEO_TRIS = TRIS_OUTPUT;
    NEO_ANSEL = ANSEL_DIGITAL;
}

//void sendBit(uint8_t bit) {
//    if (bit) {
//        NEO_PIN = 1;
//
//        __delay32(36);
//
//        NEO_PIN = 0;
//        __delay32(12);
//    } else {
//        NEO_PIN = 1;
//        __delay32(12);
//        NEO_PIN = 0;
//        __delay32(36);
//    }
//}

//void sendByte(uint8_t byte) {
//    for (int i = 7; i >= 0; i--) {
//        sendBit((byte >> i) & 0x01);
//    }
//}

//void sendPixel(uint8_t r, uint8_t g, uint8_t b) {
//    sendByte(g); // NeoPixel expects GRB order
//    sendByte(r);
//    sendByte(b);
//}
//
//void show() {
//    __delay32(8000);
//}



//////////////////////////////////




uint8_t ledBuffer[NUM_LEDS * BYTES_PER_LED];
static uint8_t globalBrightness = 255;   // 0?255 scaling

// Apply brightness scaling
static inline uint8_t scale(uint8_t v)
{
    return (uint16_t)v * globalBrightness / 255;
}

void setBrightness(uint8_t b) {
    globalBrightness = b;
}

void setPixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    if (n >= NUM_LEDS) return;

    uint16_t i = n * BYTES_PER_LED;

    // SK6812 uses GRBW order
    ledBuffer[i + 0] = scale(g);
    ledBuffer[i + 1] = scale(r);
    ledBuffer[i + 2] = scale(b);
    ledBuffer[i + 3] = scale(w);
}

void setPixelRGBW(uint16_t n, uint32_t color) {
    uint8_t r = (color >> 24) & 0xFF;
    uint8_t g = (color >> 16) & 0xFF;
    uint8_t b = (color >> 8)  & 0xFF;
    uint8_t w = (color >> 0)  & 0xFF;

    setPixel(n, r, g, b, w);
}

void clear(void) {
    for (uint16_t i = 0; i < NUM_LEDS * BYTES_PER_LED; i++)
        ledBuffer[i] = 0;
}



void switch_to_pll_80MHz(void)
{
    // Configure PLL for Fosc = 160 MHz, Fcy = 80 MHz
    CLKDIVbits.PLLPRE = 1;          // N1 = 1
    PLLFBDbits.PLLFBDIV = 40;       // M = 40
    PLLDIVbits.POST1DIV = 2;        // N2 = 2
    PLLDIVbits.POST2DIV = 1;        // N3 = 1

    // Request clock switch to FRC with PLL
    __builtin_write_OSCCONH(0x01);  // NOSC = FRCPLL
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while(OSCCONbits.OSWEN);        // Wait for switch
    while(!OSCCONbits.LOCK);        // Wait for PLL lock
}

void switch_to_frc_8MHz(void)
{
    // Unlock OSCCON
    __builtin_write_OSCCONH(0x00);  // Select FRC (no PLL)
    __builtin_write_OSCCONL(OSCCON | 0x01); // Request clock switch

    // Wait for switch
    while(OSCCONbits.OSWEN == 1);
}

void show(void) {
//    uint16_t save = __builtin_disable_interrupts();
//    switch_to_pll_80MHz();  // Fast clock for precise timing
    sk6812_sendBuffer(ledBuffer, NUM_LEDS * BYTES_PER_LED);
//    switch_to_frc_8MHz();   // Back to normal system speed
//    __builtin_restore_interrupts(save);
}
