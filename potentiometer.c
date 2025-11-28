/*
 * File:   potentiometer.c
 * Author: Osfield
 *
 * Created on November 23, 2025, 6:04 PM
 */


#include "xc.h"
#include "adc.h"
#include "potentiometer.h"
#include "config.h"
#include <stdint.h>

#define FCY 4000000UL // The 'UL' ensures it's treated as an unsigned long
#include <libpic30.h>

void POT_Init(void) {
    POT_TRIS = TRIS_INPUT;
    POT_https://github.com/osfieldgaga/curiosity-trainer/pull/3/conflict?name=potentiometer.c&base_oid=b08cdda68b6787b261bdc9601023e8aaab88a70f&head_oid=1069a71d64702b43db96d391c7c49fa6f84ea767ANSEL = ANSEL_ANALOG;

    ADC_Init();

}

int read_pot(float min, float max) {
    // start conversion for potentiometer

    int adc_val = 0;
    float output_val = 0;

    ADCON3Lbits.CNVCHSEL = 9;

    ADSTATLbits.AN9RDY = 0; //set the ready bit to 0 since we want to start conversion

    // start sampling
    ADCON3Lbits.SHRSAMP = 1;

    // wait a reasonable period of time, 100ms is much more than required
    __delay_ms(100);

    // stop sampling and start converting the sampled value
    ADCON3Lbits.SHRSAMP = 0;
    ADCON3Lbits.CNVRTCH = 1;

    while (!ADSTATLbits.AN9RDY); // wait until conversion is done

    // value is stored in ADCBUF9
    adc_val = ADCBUF9;

    // convert the result to match the required min and max
    // 4095 used for 12 bit resolution, adjust based on the resolution of
    // your ADC settings
    output_val = (((max - min) / 4095.0f) * adc_val) + min;

}
