/*
 * File:   seven_segment.c
 * Author: Osfield Gaga & Jeremy Gooch
 *
 * Created on November 23, 2025, 11:36 PM
 * 
 * Set to the seven segment module to display digits
 */


#include "xc.h"
#include "seven_segment.h"
#include "config.h"
#include <stdint.h>

/* Patterns to display
 * These patterns represent the segments to turn on, considering they are active 
 * low. It is primarily used to display numbers but any pattern can be created
 * and accessed via its array index. 
 * 
 * The patterns follow this arrangement for the different segments:
 * DP - G - F - E - D - C - B - A
 * 
 * Note that the Decimal Point (DP) is always set to 1 (OFF) by default, and is
 * rather turned ON manually. Therefore, the MSB of the pattern is always 1.
 * 
 * Pattern C0 displays the digit 0, to display 0, you need A, B, C, D, E and F
 * to be turned ON, everything else is OFF
 * 
 * 0xC0 = 0b11000000, which turns ON every segment except G (the middle segment)
 * and DP the decimal point
 */
uint8_t DIG_SEGS[11] = {0xC0, 0xF9, 0xA4, 0xB0,
    0x99, 0x92, 0x82, 0xF8,
    0x80, 0x98, 0xFF};

//the four digits to display on the four 7-seg displays
//d[3] is the leftmost, d[0] is the rightmost
uint8_t digits[4] = {0, 0, 0, 0};

/*******************************************************************************
 * Initialize the Seven Segment display, set all the pins as output
 * Display the digits 0000 by default
 * Set the "seven_seg_enable" in the config file
 ******************************************************************************/
void init_seven_seg() {

    DIG4_TRIS = TRIS_OUTPUT;
    DIG3_TRIS = TRIS_OUTPUT;
    DIG2_TRIS = TRIS_OUTPUT;
    DIG1_TRIS = TRIS_OUTPUT;

    LED_A_TRIS = TRIS_OUTPUT;
    LED_B_TRIS = TRIS_OUTPUT;
    LED_C_TRIS = TRIS_OUTPUT;
    LED_D_TRIS = TRIS_OUTPUT;
    LED_E_TRIS = TRIS_OUTPUT;
    LED_F_TRIS = TRIS_OUTPUT;
    LED_G_TRIS = TRIS_OUTPUT;
    LED_DP_TRIS = TRIS_OUTPUT;


    LED_A_ANSEL = ANSEL_DIGITAL;
    LED_B_ANSEL = ANSEL_DIGITAL;
    LED_C_ANSEL = ANSEL_DIGITAL;
    LED_D_ANSEL = ANSEL_DIGITAL;

    TRISB &= ~((1 << 10)); // Set RB10 as outputs

    print_seven_seg(0, 0, 0, 0);

    // current config doesn't have these pins on analog pins, uncomment as needed

    DIG4_ANSEL = ANSEL_DIGITAL;
    //    DIG3_ANSEL = ANSEL_DIGITAL;
    //    DIG2_ANSEL = ANSEL_DIGITAL;
    //    DIG1_ANSEL = ANSEL_DIGITAL;

    //    LED_E_ANSEL = ANSEL_DIGITAL;
    //    LED_F_ANSEL = ANSEL_DIGITAL;
    //    LED_G_ANSEL = ANSEL_DIGITAL;
    LED_DP_ANSEL = ANSEL_DIGITAL;
    
    // enable the seven segment display in the config file
    // this line is important because it enable the refresh of the seven segment
    // in the ISR
    seven_seg_enable = TRUE;
}

void print_seven_seg(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) {
    digits[0] = d0;
    digits[1] = d1;
    digits[2] = d2;
    digits[3] = d3;
}

void refresh_seven_seg() {
    //keeps track of the active digit that is currently enabled
    static uint8_t active_dig = 0; //active digit   

    uint8_t dig_code;

    //to avoid any ghosting effect, first turn off the previously active display 
    //before sending new activation values to the new display
    seven_seg_display_off();

    //send the corresponding 7-segment data to the cathode terminals
    //the 7-segment display is driven by active-low signals
    //CA is the least-significant bit of the code and DP is the most significant bit
    //assuming the code in the digits array is created with active-low values
    dig_code = DIG_SEGS[digits[active_dig]];
    LED_A = dig_code & 0x01;
    LED_B = (dig_code & 0x02) >> 1;
    LED_C = (dig_code & 0x04) >> 2;
    LED_D = (dig_code & 0x08) >> 3;
    LED_E = (dig_code & 0x10) >> 4;
    LED_F = (dig_code & 0x20) >> 5;
    LED_G = (dig_code & 0x40) >> 6;

    //activate the anode terminal for the active digit only (active low)
    //the remaining anode terminals are to remain deactivated
    switch (active_dig) {
        case 0:
            DIG1 = ANODE_ON;
            LED_DP = OFF;
            break;
        case 1:
            DIG2 = ANODE_ON;
            LED_DP = ON;
            break;
        case 2:
            DIG3 = ANODE_ON;
            LED_DP = OFF;
            break;
        case 3:
            DIG4 = ANODE_ON;
            LED_DP = OFF;
            break;
    }

    //update active_dig 
    active_dig++;
    active_dig = (active_dig > 3) ? 0 : (active_dig);
}

void seven_seg_display_off() {
    //set the cathode terminals to OFF (active low) to turn off all segments 
    LED_A = OFF;
    LED_B = OFF;
    LED_C = OFF;
    LED_D = OFF;
    LED_E = OFF;
    LED_F = OFF;
    LED_G = OFF;
    LED_DP = OFF;

    //set the anode signals to 1 (active low) to disable all displays
    DIG1 = ANODE_OFF;
    DIG2 = ANODE_OFF;
    DIG3 = ANODE_OFF;
    DIG4 = ANODE_OFF;
}
