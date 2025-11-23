/*
 * File:   config.c
 * Author: Osfield Gaga & Jeremy Gooch
 *
 * Created on November 22, 2025, 4:15 PM
 * 
 * This file configures the board and enables Timer1
 */



/******************************************************************************
 *   Configuration of the board
 *
 * These configuration were obtained through MPLAB, they are used to configure
 * the dPIC, refer to section "28.0 SPECIAL FEATURES" of the "dsPIC33CK64MC105 
 * FAMILY" datasheet for a complete overview of the registers
 * 
 * One important consideration here is to disable the JTAG interface to enable 
 * RB7 and RB10 to work as GPIOs
 *****************************************************************************/

// FSEC
#pragma config BWRP = OFF               // Boot Segment Write-Protect bit (Boot Segment may be written)
#pragma config BSS = DISABLED           // Boot Segment Code-Protect Level bits (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = DISABLED           // General Segment Code-Protect Level bits (No Protection (other than GWRP))
#pragma config CWRP = OFF               // Configuration Segment Write-Protect bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code-Protect Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table bit (Disabled AIVT)

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits (Enter Hexadecimal value)

// FOSCSEL
#pragma config FNOSC = FRC          // Oscillator Source Selection (Internal Fast RC (FRC) Oscillator with postscaler)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)
#pragma config PLLKEN = ON              // PLL Lock Enable (PLL clock output will be disabled if LOCK is lost)
#pragma config XTCFG = G3               // XT Config (24-32 MHz crystals)
#pragma config XTBST = ENABLE           // XT Boost (Boost the kick-start)

// FWDT
#pragma config RWDTPS = PS2147483648    // Run Mode Watchdog Timer Post Scaler select bits (1:2147483648)
#pragma config RCLKSEL = LPRC           // Watchdog Timer Clock Select bits (Always use LPRC)
#pragma config WINDIS = ON              // Watchdog Timer Window Enable bit (Watchdog Timer operates in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
#pragma config SWDTPS = PS2147483648    // Sleep Mode Watchdog Timer Post Scaler select bits (1:2147483648)
#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (WDT enabled in hardware)

// FPOR
#pragma config BISTDIS = DISABLED       // Memory BIST Feature Disable (mBIST on reset feature disabled)

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGC1 and PGD1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FDMTIVTL
#pragma config DMTIVTL = 0xFFFF         // Dead Man Timer Interval low word (Enter Hexadecimal value)

// FDMTIVTH
#pragma config DMTIVTH = 0xFFFF         // Dead Man Timer Interval high word (Enter Hexadecimal value)

// FDMTCNTL
#pragma config DMTCNTL = 0xFFFF         // Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)

// FDMTCNTH
#pragma config DMTCNTH = 0xFFFF         // Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)

// FDMT
#pragma config DMTDIS = OFF             // Dead Man Timer Disable bit (Dead Man Timer is Disabled and can be enabled by software)

// FDEVOPT
#pragma config ALTI2C1 = OFF            // Alternate I2C1 Pin bit (I2C1 mapped to SDA1/SCL1 pins)
#pragma config SMB3EN = SMBUS3          // SM Bus Enable (SMBus 3.0 input levels)
#pragma config SPI2PIN = PPS            // SPI2 Pin Select bit (SPI2 uses I/O remap (PPS) pins)

// FALTREG
#pragma config CTXT1 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits (Not Assigned)
#pragma config CTXT2 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (Not Assigned)
#pragma config CTXT3 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits (Not Assigned)
#pragma config CTXT4 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits (Not Assigned)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "config.h"
#include "utils.h"
#include <stdint.h>

uint8_t seven_seg_enable = FALSE;

/******************************************************************************
 *   Set up Timer 1
 * 
 * This comment block encompasses both the interrupt definition and init_T1()
 *
 * Timer 1 is used as a free-running timer throughout. Since the dsPIC33CK64MC105
 * does not have a core timer, T1 is set to trigger an interrupt every 1 ms.
 * 
 * The ISR sets a tFlag, which is by default set to 1 second. This can be configu-
 * red in the config.h file to change the period
 * 
 * The ISR is also used to refresh the seven segment display, if enabled.
 * The enable flag of the seven segment is used to ensure the refreshing only
 * happens when the seven segment display is explicitely enabled by the user.
 * This serves two purposes: prevent the ISR from running code that is not needed,
 * and ensure the pins aren't being written unnecessarily, considering other
 * devices may share the seven segment pins
 *****************************************************************************/

uint16_t msCount = 0;
uint8_t tFlag = 0;

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {

    msCount++;

    if (seven_seg_enable) {
        refresh_seven_seg();
    }

    if (msCount >= T1_TIME_MS) {
        tFlag = 1;
        msCount = 0;
    }

    if (enDelay) {
        // delayCounterMs is an extern coming from utils.c/h
        // it is used as the delay counter, calling the delay function sets
        // this value and counts down
        delayCounterMs--;
    }

    // clear interrupt flag
    IFS0bits.T1IF = 0;
}

void init_T1() {

    TMR1 = 0;
    PR1 = T1_PERIOD; // 1ms

    T1CONbits.TCKPS = 0x01; //1:8 prescaler

    T1CONbits.TCS = 0; // Use internal clock

    // enable interrupts
    IPC0bits.T1IP = 4;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

    //turn on timer 1
    T1CONbits.TON = 1;
}
