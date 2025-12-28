/*
 * File:   switches.c
 * Author: Osfield
 *
 * Created on December 3, 2025, 12:25 PM
 */


#include "xc.h"
#include "switches.h"
#include "config.h"
#include <stdint.h>

void SW_Init() {
    SW1_TRIS = TRIS_INPUT;
    SW2_TRIS = TRIS_INPUT;
    SW3_TRIS = TRIS_INPUT;
    SW4_TRIS = TRIS_INPUT;
    SW5_TRIS = TRIS_INPUT;
    SW6_TRIS = TRIS_INPUT;
    SW7_TRIS = TRIS_INPUT;
    SW8_TRIS = TRIS_INPUT;

//    SW1_ANSEL = ANSEL_DIGITAL;
//    SW2_ANSEL = ANSEL_DIGITAL;
    SW3_ANSEL = ANSEL_DIGITAL;
    SW4_ANSEL = ANSEL_DIGITAL;
//    SW5_ANSEL = ANSEL_DIGITAL;
//    SW6_ANSEL = ANSEL_DIGITAL;
//    SW7_ANSEL = ANSEL_DIGITAL;
//    SW8_ANSEL = ANSEL_DIGITAL;
}

uint8_t read_switch(uint8_t index) {
    uint8_t out;

    switch (index) {
        case 1:
            out = SW1;
            break;
        case 2:
            out = SW2;
            break;
        case 3:
            out = SW3;
            break;
        case 4:
            out = SW4;
            break;
        case 5:
            out = SW5;
            break;
        case 6:
            out = SW6;
            break;
        case 7:
            out = SW7;
            break;
        case 8:
            out = SW8;
            break;
        default:
            break;
    }

    return out;
}

uint8_t read_switches() {
    uint8_t out = ((SW8 << 7) | (SW7 << 6) | (SW6 << 5) | (SW5 << 4) | (SW4 << 3)
            | (SW3 << 2) | (SW2 << 1) | (SW1));
    return out;
}


