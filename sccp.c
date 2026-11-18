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
    
    // PPS must be unlocked first: other drivers (e.g. PWM4_Disable()) lock it,
    // and writes to RPINRx are silently ignored while it is locked
    __builtin_write_RPCON(0x0000); // unlock PPS
    RPINR3bits.ICM1R =  40; //RP40, or RB8
    __builtin_write_RPCON(0x0800); // lock PPS
    
    CCP1CON1Lbits.CCPON = 0; // Disable CCP/input capture first
    
    CCP1CON1Lbits.CCSEL = 1; // Input capture mode
    CCP1CON1Lbits.CLKSEL = 0; // Set the clock source (Tcy = 16 MHz)
    CCP1CON1Lbits.T32 = 0; // 16-bit Dual Timer mode
    CCP1CON1Lbits.MOD = 3; // Capture ever rising and falling edge of the event
//    CCP1CON2Hbits.ICSEL = 0; // Capture rising edge on the Pin
//    CCP1CON1Hbits.IOPS = 0; // Interrupt on every input capture event
    // 1:16 pre-scaler: 16 MHz / 16 = 1 MHz, so 1 tick = 1 us. The 16-bit timer
    // wraps every 65.5 ms, longer than the longest echo (~38 ms, no obstacle)
    CCP1CON1Lbits.TMRPS = 2;
    CCP1CON1Lbits.CCPON = 1; // Enable CCP/input capture
}
