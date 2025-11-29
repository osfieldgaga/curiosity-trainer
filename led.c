/*
 * File:   led.c
 * Author: Osfield
 *
 * Created on November 28, 2025, 7:52 PM
 */


#include "xc.h"
#include "led.h"
#include "config.h"
#include "stdint.h"


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



    // Ansell

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

    RGB_R = LED_OFF;
    RGB_G = LED_OFF;
    RGB_B = LED_OFF;

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

void set_all_leds_state(uint8_t state) {

    for (uint8_t i = 0; i < 8; i++) {
        set_led_state(i, state);
    }
}

