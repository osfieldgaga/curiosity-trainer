/*
 * File:   potentiometer.c
 * Author: Osfield
 *
 * Created on November 27, 2025, 9:38 PM
 */


#include "xc.h"
#include "adc.h"
#include "potentiometer.h"
#include "config.h"

#define FCY 4000000UL // The 'UL' ensures it's treated as an unsigned long
#include <libpic30.h>

void POT_Init(void) {
    POT_TRIS = TRIS_INPUT;
    POT_ANSEL = ANSEL_ANALOG;

    ADC_Init();

}

int read_pot() {
    // start conversion for potentiometer

    ADCON3Lbits.CNVCHSEL = 9;

    ADSTATLbits.AN9RDY = 0; //set the ready bit to 0 since we want to start conversion

    // 2. START SAMPLING (Manually)
    ADCON3Lbits.SHRSAMP = 1;

    // 3. WAIT FOR MINIMUM SAMPLE TIME (Manual Delay Required)
    // This delay ensures the internal capacitor is fully charged.
    // Replace with a function that executes the correct number of NOPs or cycles.
    __delay_ms(100);

    // 4. START CONVERSION (Manual Trigger)
    // Clear SHRSAMP to stop sampling and set CNVRTCH to start conversion.
    ADCON3Lbits.SHRSAMP = 0;
    ADCON3Lbits.CNVRTCH = 1;

    while (!ADSTATLbits.AN9RDY); // wait until conversion is done

    return ADCBUF9;
}
