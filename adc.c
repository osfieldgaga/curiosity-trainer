/*
 * 
 *  @Company
        Kettering University
 * 
 *  @File Name
 *      adc.c
 * 
 *  @Summary
 *      Initialization of the shared ADC module.
 * 
 *  @Description
 *      Set up the ADC module and wait until it is ready.
 *      Setting up the ADC does not automatically assign the channel to use, this
 *      initialization is meant to be wrapped in a function that explicitely specifies
 *      the channel to be used for a specific application or component.
 * 
 *  @Authors: 
 *      Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 27, 2025, 9:35 PM
 * 
 *  @Last modification
 *      December 08, 2025, 11:50 PM
 */


#include "xc.h"
#include "adc.h"
#include "config.h"
#include <libpic30.h>



/* Initialize ADC module
 * 
 * This function initializes the shared ADC Core
 * 
 * Example:
 * 
 * Choosing channel 9 as the input channel can be done with:
 * ADCON3Lbits.CNVCHSEL = 9;
 * 
 * The ANxRDY bit must be cleared to prevent unexpected behavior
 * 
 * ADSTATLbits.AN9RDY = 0; 
 */
int ADC_Init(void) {

    ADCON1Lbits.ADON = 0; // turn off everything before setting up

    ADCON3Hbits.CLKSEL = 0; // selects the peripheral clock as the source

    // creates the main Tcoresrc that will be used by the other dedicated ADC cores
    // via Tadcore. Here, 0, means 1 source clock period (no division)
    ADCON3Hbits.CLKDIV = 0;

    ADCON3Lbits.REFSEL = 0; // voltage reference

    ADCON1Hbits.FORM = 0; //output the value as an integer

    ADCON2Hbits.SHRSAMC = 0; // sampling time is 2 x Tadcore for the shared ADC 

    ADCON1Lbits.ADON = 1; //turn ON the ADC

    ADCON5Hbits.WARMTIME = 8;

    ADCON5Lbits.SHRPWR = 1; //enable the ADC core

    while (!ADCON5Lbits.SHRRDY); // wait until the ADC is initialized and ready to be used

    ADCON3Hbits.SHREN = 1; // now enable the ADC core

}

int ADC_Core_Read() {
    // start sampling
    ADCON3Lbits.SHRSAMP = 1;

    // wait a reasonable period of time, 100ms is much more than required
    __delay_ms(25);

    // stop sampling and start converting the sampled value
    ADCON3Lbits.SHRSAMP = 0;
    ADCON3Lbits.CNVRTCH = 1;

    while (!ADSTATLbits.AN9RDY); // wait until conversion is done

    return ADCBUF9;
}

void ADC_Set_AN9() {
    ADCON3Lbits.CNVCHSEL = 9;

    ADSTATLbits.AN9RDY = 0; //set the ready bit to 0 since we want to start conversion
}