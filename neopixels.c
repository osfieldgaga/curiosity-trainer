/*
 * File:   neopixels.c
 * Author: Osfield
 *
 * Created on December 17, 2025, 1:34 PM
 */


#include "xc.h"
#include "neopixels.h"
#include "config.h"
#include <libpic30.h>

void NEO_Init() {
    NEO_TRIS = TRIS_OUTPUT;
    NEO_ANSEL = ANSEL_DIGITAL;
}

void sendBit(uint8_t bit) {
    if (bit) {
        NEO_PIN = 1;

        __delay32(36);

        NEO_PIN = 0;
        __delay32(12);
    } else {
        NEO_PIN = 1;
        __delay32(12);
        NEO_PIN = 0;
        __delay32(36);
    }
}

void sendByte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        sendBit((byte >> i) & 0x01);
    }
}

void sendPixel(uint8_t r, uint8_t g, uint8_t b) {
    sendByte(g); // NeoPixel expects GRB order
    sendByte(r);
    sendByte(b);
}

void show() {
    __delay32(8000);
}
