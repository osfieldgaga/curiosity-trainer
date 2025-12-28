/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    potentiometer.h
 *
 *  @Summary
 *    Defines function prototypes for the potentiometer
 *
 *  @Description
 *    The functions used for interfacing the potentiometer and the function prototypes
 *    are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 23, 2025, 6:04 PM
 * 
 *  @Last modification
 *      December 09, 2025, 5:23 PM
/* ************************************************************************** */

#include "xc.h"
#include "adc.h"
#include "potentiometer.h"
#include "config.h"
#include <stdint.h>

#define FCY 4000000UL // The 'UL' ensures it's treated as an unsigned long
#include <libpic30.h>

// initialize the potentiometer pins
void POT_Init(void) {
    POT_TRIS = TRIS_INPUT;
    POT_ANSEL = ANSEL_ANALOG;

    ADC_Init();

}

// read the value of the the potentiometer
int read_pot(float min, float max) {
    // start conversion for potentiometer

    int adc_val = 0;
    float output_val = 0;

    ADC_Set_AN9();

    // value is stored in ADCBUF9
    adc_val = ADC_Core_Read();

    // convert the result to match the required min and max
    // 4095 used for 12 bit resolution, adjust based on the resolution of
    // your ADC settings
    output_val = (((max - min) / 4095.0f) * adc_val) + min;
    
    return output_val;

}
