/*
 * File:   sccp.c
 * Author: Osfield
 *
 * Created on December 10, 2025, 3:02 PM
 */


#include <p33CK64MC105.h>

#include "xc.h"
#include "config.h"
#include "sccp.h"


void SCCP_Init() {
    
    RPINR3bits.ICM1R =  40; //RP40, or RB8
    
    CCP1CON1Lbits.CCPON = 0; // Disable CCP/input capture first
    
    CCP1CON1Lbits.CCSEL = 1; // Input capture mode
    CCP1CON1Lbits.CLKSEL = 0; // Set the clock source (Tcy)
    CCP1CON1Lbits.T32 = 0; // 16-bit Dual Timer mode
    CCP1CON1Lbits.MOD = 3; // Capture ever rising and falling edge of the event
//    CCP1CON2Hbits.ICSEL = 0; // Capture rising edge on the Pin
//    CCP1CON1Hbits.IOPS = 0; // Interrupt on every input capture event
    CCP1CON1Lbits.TMRPS = 0; // Set the clock pre-scaler (1:1)
    CCP1CON1Lbits.CCPON = 1; // Enable CCP/input capture
}
