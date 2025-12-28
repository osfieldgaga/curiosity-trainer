/*
 * 
 *  @Company
        Kettering University
 * 
 *  @File Name
 *      photoresistor.c
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
 *      November 27, 2025, 9:38 PM
 * 
 *  @Last modification
 *      December 09, 2025, 12:17 PM
 */


#include "xc.h"
#include "adc.h"
#include "photoresistor.h"
#include "config.h"
#include <stdint.h>

#include <libpic30.h>

// initialize the photoresistor pins
void LDR_Init(void) {
    LDR_TRIS = TRIS_INPUT;
    LDR_ANSEL = ANSEL_ANALOG;

    ADC_Init();

}

// read the value of the the photoresistor
int read_ldr(float min, float max) {
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

    return output_val;
}
