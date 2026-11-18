#ifndef SK6812_H
#define SK6812_H

#include <stdint.h>

#define NUM_LEDS  4        // <-- set your LED count here
#define BYTES_PER_LED 4     // GRBW

extern void sk6812_sendBuffer(uint8_t *buf, uint16_t length);

// Pixel buffer (GRBW)
extern uint8_t ledBuffer[NUM_LEDS * BYTES_PER_LED];

// API
void setPixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void setPixelRGBW(uint16_t n, uint32_t color);
void clear(void);
void show(void);
void setBrightness(uint8_t b);

#endif