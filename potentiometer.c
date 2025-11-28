/*
 * File:   potentiometer.c
 * Author: Osfield
 *
 * Created on November 23, 2025, 6:04 PM
 */


#include "xc.h"
#include "potentiometer.h"
#include "config.h"

void init_pot(void) {
    POT_TRIS = TRIS_INPUT;
    POT_ANSEL = ANSEL_ANALOG;
    
    ADCON1Lbits.ADON = 1;
    ADCMP0CONbits.CHNL = 0x09; //AN 9 = RA2
    
}
