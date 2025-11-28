/*
 * File:   adc.c
 * Author: Osfield
 *
 * Created on November 27, 2025, 9:35 PM
 */


#include "xc.h"
#include "adc.h"

int ADC_Init(void) {

    ADCON1Lbits.ADON = 0; // turn off everything before setting uo

    ADCON3Hbits.CLKSEL = 0; // selects the peripheral clock as the source
    ADCON3Hbits.CLKDIV = 0; // no clock divider (1:1)


    // creates the main Tcoresrc that will be used by the other dedicated ADC cores
    // via Tadcore. Here, 0, means 1 source clock period (no division)
    ADCON3Hbits.CLKDIV = 0;

    ADCON3Lbits.REFSEL = 0; // voltage reference, AFAIK can only be set to 0

    ADCON1Hbits.FORM = 0; //output the value as an integer


    ADCON2Hbits.SHRSAMC = 0; // sampling time is 2 x Tadcore for the shared ADC 

    ADCON1Lbits.ADON = 1; //turn ON the ADC
    
    ADCON5Hbits.WARMTIME = 8;

    ADCON5Lbits.SHRPWR = 1; //enable the ADC core

    while (!ADCON5Lbits.SHRRDY); // wait until the ADC is initialized and ready to be used

    ADCON3Hbits.SHREN = 1; // now enable the ADC core

}