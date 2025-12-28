/*
 * 
 *  @Company
        Kettering University
 * 
 *  @File Name
 *      led.c
 * 
 *  @Summary
 *      This file sets up the LEDs and provides functions to interact with them 
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

#include "xc.h"
#include "led.h"
#include "config.h"
#include "stdint.h"
#include <libpic30.h>

void LED_Init(void) {
    // Tris

    LED0_TRIS = TRIS_OUTPUT;
    LED1_TRIS = TRIS_OUTPUT;
    LED2_TRIS = TRIS_OUTPUT;
    LED3_TRIS = TRIS_OUTPUT;
    LED4_TRIS = TRIS_OUTPUT;
    LED5_TRIS = TRIS_OUTPUT;
    LED6_TRIS = TRIS_OUTPUT;
    LED7_TRIS = TRIS_OUTPUT;

    RGB_R_TRIS = TRIS_OUTPUT;
    RGB_G_TRIS = TRIS_OUTPUT;
    RGB_B_TRIS = TRIS_OUTPUT;



    // Ansel

    LED0_ANSEL = ANSEL_DIGITAL;
    LED1_ANSEL = ANSEL_DIGITAL;
    //    LED2_ANSEL = ANSEL_DIGITAL;
    //    LED3_ANSEL = ANSEL_DIGITAL;
    LED4_ANSEL = ANSEL_DIGITAL;
    LED5_ANSEL = ANSEL_DIGITAL;
    LED6_ANSEL = ANSEL_DIGITAL;
    LED7_ANSEL = ANSEL_DIGITAL;

    //    RGB_R_ANSEL = ANSEL_DIGITAL;
    //    RGB_G_ANSEL = ANSEL_DIGITAL;
    //    RGB_B_ANSEL = ANSEL_DIGITAL;


    // turn off all LEDs to begin with

    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;
    LED4 = LED_OFF;
    LED5 = LED_OFF;
    LED6 = LED_OFF;
    LED7 = LED_OFF;

    RGB_R = RGB_OFF;
    RGB_G = RGB_OFF;
    RGB_B = RGB_OFF;

}

void turn_off_leds() {
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;
    LED4 = LED_OFF;
    LED5 = LED_OFF;
    LED6 = LED_OFF;
    LED7 = LED_OFF;
}

void set_led_state(uint8_t index, uint8_t state) {

    // Determine the state value (e.g., if LED_ON is 0, set bit to 0)

    switch (index) {
        case 0:
            LED0 = state;
            break;
        case 1:
            LED1 = state;
            break;
        case 2:
            LED2 = state;
            break;
        case 3:
            LED3 = state;
            break;
        case 4:
            LED4 = state;
            break;
        case 5:
            LED5 = state;
            break;
        case 6:
            LED6 = state;
            break;
        case 7:
            LED7 = state;
            break;
        default:

            break;
    }
}


void set_all_leds(uint8_t state) {

    LED7 = (state >> 7) & 0x01;
    LED6 = (state >> 6) & 0x01;
    LED5 = (state >> 5) & 0x01;
    LED4 = (state >> 4) & 0x01;
    LED3 = (state >> 3) & 0x01;
    LED2 = (state >> 2) & 0x01;
    LED1 = (state >> 1) & 0x01;
    LED0 = (state) & 0x01;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    void turn_off_rgb();
        __delay_ms(20);

    if (r) {
        RGB_R = RGB_ON;
        RGB_G = RGB_OFF;
        RGB_B = RGB_OFF;
    }

    if (g) {
        RGB_R = RGB_OFF;
        RGB_G = RGB_ON;
        RGB_B = RGB_OFF;
    }

    if (b) {
        RGB_R = RGB_OFF;
        RGB_G = RGB_ON;
        RGB_B = RGB_ON;
    }

}

void turn_off_rgb() {

    RGB_R = RGB_OFF;
    RGB_G = RGB_OFF;
    RGB_B = RGB_OFF;


}

