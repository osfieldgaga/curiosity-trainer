/*
 * File:   ultrasonic.c
 * Author: Osfield
 *
 * Created on December 10, 2025, 3:01 PM
 */


#include "xc.h"
#include "config.h"
#include "ultrasonic.h"
#include "sccp.h"
#include <libpic30.h>

void init_ultrasonic() {
    

    TRIGGER_TRIS = TRIS_OUTPUT;
    ECHO_TRIS = TRIS_INPUT;

    TRIGGER_ANSEL = ANSEL_DIGITAL;
    ECHO_ANSEL = ANSEL_DIGITAL;

    TRIGGER = 0;
    
    SCCP_Init();
}

int sense_distance() {

    int t1, t2, ticks;
    float pulseWidth_us, distance;

    // send trigger
    TRIGGER = 1;
    __delay_ms(1);
    TRIGGER = 0;

    __delay_ms(50);

    t1 = CCP1BUFL;
    t2 = CCP1BUFL;

    ticks = t2 - t1;

    pulseWidth_us = ticks * 0.25f;

    // according to the datasheet, 150 us is the minimum pulse width
    if (pulseWidth_us < 150) {
        distance = 0;
    } else {
        distance = pulseWidth_us / 58.0f;
    }


    return distance;
}
