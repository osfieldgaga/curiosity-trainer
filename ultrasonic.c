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

    uint16_t t1, t2, ticks;
    float distance;

    // discard any capture left over from a previous measurement or noise, so
    // the next two captures are the rising and falling edges of this echo
    while (CCP1STATLbits.ICBNE) {
        (void) CCP1BUFL;
    }
    CCP1STATLbits.ICOV = 0;

    // send trigger
    TRIGGER = 1;
    __delay_ms(1);
    TRIGGER = 0;

    __delay_ms(50);

    // rising edge of the echo, none means no echo (sensor not connected)
    if (!CCP1STATLbits.ICBNE) {
        return 7777;
    }
    t1 = CCP1BUFL;

    // falling edge of the echo
    if (!CCP1STATLbits.ICBNE) {
        return 8888;
    }
    t2 = CCP1BUFL;

    // the timer runs at 1 MHz (see SCCP_Init()), so ticks are in us. The
    // unsigned subtraction stays correct if the timer wrapped between the edges
    ticks = t2 - t1;

    // according to the datasheet, 150 us is the minimum pulse width
    if (ticks < 150) {
        distance = 9999;
    } else {
        distance = ticks / 58.0f; // 58 us per cm, round trip
    }

    return distance;
}
