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
    
    LATBbits.LATB9 = 0;
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




/*
    Main application
*/

/**
 * @brief Sends a byte of data serially using the send1() macro.
 *
 * This function transmits an 8-bit byte, most significant bit (MSB) first,
 * by calling the `send1()` macro for each bit.  It uses bitwise AND operations
 * to isolate each bit of the input byte.
 *
 * @param b The 8-bit byte to be transmitted.
 *
 * @note This function relies on the `send1()` macro being defined and properly
 *       configured for the target hardware and communication protocol.
 *
 * Example Usage:
 *   sendByte(0x5A);  // Sends the byte 0x5A (binary 01011010)
 */
//void sendByte(uint8_t b){
//    if (b & 0b10000000) { send1(1); } else { send1(0); }
//    if (b & 0b01000000) { send1(1); } else { send1(0); }
//    if (b & 0b00100000) { send1(1); } else { send1(0); }
//    if (b & 0b00010000) { send1(1); } else { send1(0); }
//    if (b & 0b00001000) { send1(1); } else { send1(0); }
//    if (b & 0b00000100) { send1(1); } else { send1(0); }
//    if (b & 0b00000010) { send1(1); } else { send1(0); }
//    if (b & 0b00000001) { send1(1); } else { send1(0); }  
//}

void sendByte(uint8_t b){
    if (b & 0b10000000) { send1(); } else { send0(); }
    if (b & 0b01000000) { send1(); } else { send0(); }
    if (b & 0b00100000) { send1(); } else { send0(); }
    if (b & 0b00010000) { send1(); } else { send0(); }
    if (b & 0b00001000) { send1(); } else { send0(); }
    if (b & 0b00000100) { send1(); } else { send0(); }
    if (b & 0b00000010) { send1(); } else { send0(); }
    if (b & 0b00000001) { send1(); } else { send0(); }  
}

/**
 * @brief Sends RGB color data serially.
 *
 * This function transmits 24 bits of color data (8 bits each for green, red, and blue)
 * by calling `sendByte()` for each color component.  The order is Green, Red, Blue (GRB).
 *
 * @param r The 8-bit red color component (0-255).
 * @param g The 8-bit green color component (0-255).
 * @param b The 8-bit blue color component (0-255).
 *
 * @note  The order of color components (GRB) is important and might be different
 *        from the expected RGB order.  This is a common convention for some addressable LEDs.
 *
 * Example Usage:
 *   sendRGB(255, 0, 128);  // Sends data for full red, no green, and half blue.
 */
void sendRGB (uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    sendByte(g);    
    sendByte(r);
    sendByte(b);    
    sendByte(w);    
}

/**
 * @brief Sends RGB color data multiple times serially.
 *
 * This function repeatedly transmits the specified RGB color data a given number of times.
 * It's likely used to control a chain of addressable LEDs, where each repetition sends
 * the same color data to the next LED in the chain.
 *
 * @param red   The 8-bit red color component (0-255).
 * @param green The 8-bit green color component (0-255).
 * @param blue  The 8-bit blue color component (0-255).
 * @param count The number of times to repeat the RGB data transmission.  This usually
 *              corresponds to the number of LEDs in a chain.
 *
 * @note This function relies on the `sendRGB()` function, which in turn relies on `sendByte()`
 *       and `send1()`.  The timing of the underlying `send1()` macro is critical for
 *       correct operation.
 *
 * Example Usage:
 *   sendRGBMultiple(0, 255, 0, 10);  // Sends data for full green to 10 LEDs.
 */
void sendRGBMultiple(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, int count) {
    // the bit timing relies on Nop() counts, an interrupt (e.g. the T1 ISR)
    // in the middle of a bit would corrupt the colors. A pending interrupt is
    // simply served once the transfer is done
    INTCON2bits.GIE = 0;

    for (int i = 0; i < count; i++) {
        sendRGB(red, green, blue, white);
    }

    INTCON2bits.GIE = 1;
}
/**
 * @brief Executes a pattern by progressively increasing the number of LEDs lit with a specific color.
 *
 * This function creates a visual pattern where an increasing number of LEDs are lit with the
 * specified RGB color.  It starts by lighting one LED, then two, and so on, up to `maxCount` LEDs.
 * A delay is introduced between each step to make the pattern visible.
 *
 * @param red      The 8-bit red color component (0-255).
 * @param green    The 8-bit green color component (0-255).
 * @param blue     The 8-bit blue color component (0-255).
 * @param maxCount The maximum number of LEDs to light in the pattern.  This determines
 *                 the length of the sequence and the maximum number of LEDs that will be lit
 *                 simultaneously.
 *
 * @note This function relies on the `sendRGBMultiple()` function, which in turn depends on
 *       `sendRGB()`, `sendByte()`, and `send1()`.  The timing of the `send1()` macro is
 *       critical for correct operation.  The `__delay_ms()` function is assumed to be
 *       defined and provide a millisecond delay.  The accuracy of the delay depends on
 *       the microcontroller's clock frequency and the implementation of `__delay_ms()`.      
 *
 * Example Usage:
 *   performPattern(0, 0, 255, 5);  // Creates a pattern where 1 to 5 LEDs are lit blue,
 *                                  // with a 500ms delay between each step.
 */
void performPattern(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, int maxCount) {
    
    for (int i = 1; i <= maxCount; i++) 
    {
        sendRGBMultiple(red, green, blue, white, i);
//        __delay_ms(5);
    }        
    
}



//uint8_t ledBuffer[NUM_LEDS * BYTES_PER_LED];
//static uint8_t globalBrightness = 255;   // 0?255 scaling
//
//// Apply brightness scaling
//static inline uint8_t scale(uint8_t v)
//{
//    return (uint16_t)v * globalBrightness / 255;
//}
//
//void setBrightness(uint8_t b) {
//    globalBrightness = b;
//}
//
//void setPixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
//    if (n >= NUM_LEDS) return;
//
//    uint16_t i = n * BYTES_PER_LED;
//
//    // SK6812 uses GRBW order
//    ledBuffer[i + 0] = scale(g);
//    ledBuffer[i + 1] = scale(r);
//    ledBuffer[i + 2] = scale(b);
//    ledBuffer[i + 3] = scale(w);
//}
//
//void setPixelRGBW(uint16_t n, uint32_t color) {
//    uint8_t r = (color >> 24) & 0xFF;
//    uint8_t g = (color >> 16) & 0xFF;
//    uint8_t b = (color >> 8)  & 0xFF;
//    uint8_t w = (color >> 0)  & 0xFF;
//
//    setPixel(n, r, g, b, w);
//}
//
//void clear(void) {
//    for (uint16_t i = 0; i < NUM_LEDS * BYTES_PER_LED; i++)
//        ledBuffer[i] = 0;
//}
//
//
//
//void switch_to_pll_80MHz(void)
//{
//    // Configure PLL for Fosc = 160 MHz, Fcy = 80 MHz
//    CLKDIVbits.PLLPRE = 1;          // N1 = 1
//    PLLFBDbits.PLLFBDIV = 40;       // M = 40
//    PLLDIVbits.POST1DIV = 2;        // N2 = 2
//    PLLDIVbits.POST2DIV = 1;        // N3 = 1
//
//    // Request clock switch to FRC with PLL
//    __builtin_write_OSCCONH(0x01);  // NOSC = FRCPLL
//    __builtin_write_OSCCONL(OSCCON | 0x01);
//
//    while(OSCCONbits.OSWEN);        // Wait for switch
//    while(!OSCCONbits.LOCK);        // Wait for PLL lock
//}
//
//void switch_to_frc_8MHz(void)
//{
//    // Unlock OSCCON
//    __builtin_write_OSCCONH(0x00);  // Select FRC (no PLL)
//    __builtin_write_OSCCONL(OSCCON | 0x01); // Request clock switch
//
//    // Wait for switch
//    while(OSCCONbits.OSWEN == 1);
//}
//
//void show(void) {
////    uint16_t save = __builtin_disable_interrupts();
////    switch_to_pll_80MHz();  // Fast clock for precise timing
//    sk6812_sendBuffer(ledBuffer, NUM_LEDS * BYTES_PER_LED);
////    switch_to_frc_8MHz();   // Back to normal system speed
////    __builtin_restore_interrupts(save);
//}
