#include <xc.h>
#include <stdint.h>
#include "pwm.h"

void PWM1_Initialize(void) {
    // Reset control registers for a clean start
    PG1CONL = 0x0000;
    PG1CONH = 0x0000;
    PG1IOCONH = 0x0000;

    // --- Control Setup ---
    PG1CONLbits.CLKSEL = 0b01; // CLKSEL: master clock (PCLKCON.MCLKSEL, Fosc = 32 MHz by default), not divided
    PG1CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode
    PG1CONHbits.MDCSEL = 0; // MDCS: Use PG1DC register for duty cycle value

    // --- Output Pin Enable ---
    PG1IOCONHbits.PENL = 1; // PENL: Enable PWM1L (Low-side) output pin

    // --- Period and Duty Cycle Setting ---
    // the counter runs at 32 MHz: frequency = 32 MHz / (PG1PER + 1)
    PG1PER = 10000; // PERIOD: 32 MHz / 10001 = 3.2 kHz
    PG1DC = 500; // DUTY CYCLE: 500 / 10001 = 5%

    // --- Enable PWM ---
    PG1CONLbits.ON = 1; // Turn ON PWM Generator 1
}

void PWM2_Initialize(void) {
    // Reset control registers for a clean start
    PG2CONL = 0x0000;
    PG2CONH = 0x0000;
    PG2IOCONH = 0x0000;

    // --- Control Setup ---
    PG2CONLbits.CLKSEL = 0b01; // CLKSEL: master clock (PCLKCON.MCLKSEL, Fosc = 32 MHz by default), not divided

    //MODSEL[2:0]: Mode Selection bits
    //111 = Dual Edge Center-Aligned PWM mode (interrupt/register update twice per cycle)
    //110 = Dual Edge Center-Aligned PWM mode (interrupt/register update once per cycle)
    //101 = Double-Update Center-Aligned PWM mode
    //100 = Center-Aligned PWM mode
    //011 = Reserved
    //010 = Independent Edge PWM mode, dual output
    //001 = Variable Phase PWM mode
    //000 = Independent Edge PWM mode
    PG2CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode

    //MDCSEL: Master Duty Cycle Register Select bit
    //1 = PWM Generator uses MDC register
    //0 = PWM Generator uses PGxDC register
    PG2CONHbits.MDCSEL = 0; // MDCS: Use PG2DC register for duty cycle value

    //MCLKSEL[1:0]: PWM Master Clock Selection bits(2)
    //11 = FVCO/3
    //10 = FPLLO ? Primary PLL post-divider output
    //01 = FVCO/2
    //00 = FOSC
    PCLKCONbits.MCLKSEL = 0;

    // --- Output Pin Enable ---
    PG2IOCONHbits.PENH = 0; // PENL: Enable PWMxH (High-side) output pin
    PG2IOCONHbits.PENL = 1; // PENL: Enable PWMxL (Low-side) output pin

    // --- Period and Duty Cycle Setting ---
    // the counter runs at 32 MHz: frequency = 32 MHz / (PG2PER + 1)
    PG2PER = 15286; // PERIOD: 32 MHz / 15287 = 2.09 kHz
    PG2DC = PG2PER / 2; // DUTY CYCLE: 50%

    // --- Enable PWM ---
    PG2CONLbits.ON = 1; // Turn ON PWM Generator 2
}

void PWM3_Initialize(void) {
    // Reset control registers for a clean start
    PG3CONL = 0x0000;
    PG3CONH = 0x0000;
    PG3IOCONH = 0x0000;

    // --- Control Setup ---
    PG3CONLbits.CLKSEL = 0b01; // CLKSEL: master clock (PCLKCON.MCLKSEL, Fosc = 32 MHz by default), not divided
    PG3CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode
    PG3CONHbits.MDCSEL = 0; // MDCS: Use PG3DC register for duty cycle value

    // --- Output Pin Enable ---
    PG3IOCONHbits.PENL = 1; // PENL: Enable PWM3L (Low-side) output pin

    // --- Period and Duty Cycle Setting ---
    // the counter runs at 32 MHz: frequency = 32 MHz / (PG3PER + 1)
    PG3PER = 15286; // PERIOD: 32 MHz / 15287 = 2.09 kHz
    PG3DC = 500; // DUTY CYCLE: 500 / 15287 = 3%

    // --- Enable PWM ---
    PG3CONLbits.ON = 1; // Turn ON PWM Generator 3
}

void PWM4_Initialize(void) {

    // first, map PWM4 to RB7

    // Unlock PPS registers 
    __builtin_write_RPCON(0x0000);


    // RB7 is RP39
    RPOR3bits.RP39R = PWM4L_OFC;

    // Lock PPS registers
    __builtin_write_RPCON(0x0800);

    // Configure RD8 as a Digital Output (0 = Output)
    TRISDbits.TRISD8 = 0;

    // then setup the pWM module

    // Reset control registers for a clean start
    PG4CONL = 0x0000;
    PG4CONH = 0x0000;
    PG4IOCONH = 0x0000;

    // --- Control Setup ---
    // PWM4 counts at Fosc / 16 = 32 MHz / 16 = 2 MHz (FP in pwm.h), so the
    // 16-bit PG4PER covers the buzzer notes down to ~31 Hz
    PCLKCONbits.MCLKSEL = 0; // master clock = Fosc (32 MHz)
    PCLKCONbits.DIVSEL = 0b11; // master clock divider 1:16
    PG4CONLbits.CLKSEL = 0b10; // CLKSEL: master clock divided by the clock divider
    PG4CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode
    PG4CONHbits.MDCSEL = 0; // MDCS: Use PG4DC register for duty cycle value

    // --- Output Pin Enable ---
    PG4IOCONHbits.PENL = 1; // PENL: Enable PWM4L (Low-side) output pin

    // --- Period and Duty Cycle Setting ---
    PG4PER = 15286; // 2 MHz / 15287 = 130.8 Hz (C3)
    PG4DC = PG4PER / 2; // DUTY CYCLE: Set to 50% 

    // --- Enable PWM ---
    PG4CONLbits.ON = 1; // Turn ON PWM Generator 4
}

void PWM4_Disable(void) {



    // 1. Disable PWM Module first
    PG4CONLbits.ON = 0;
    PG4IOCONHbits.PENL = 0;

    // 2. Unlock PPS 
    __builtin_write_RPCON(0x0000);

    // 3. Clear the mapping for RB7 (RP39)
    RPOR3bits.RP39R = 0;

    // 4. Lock PPS
    __builtin_write_RPCON(0x0800);

    // 5. CRITICAL: Switch pin from Analog to Digital
    ANSELBbits.ANSELB7 = 1; // 

    // 6. Set GPIO Direction
    TRISBbits.TRISB7 = 1; // 0 = Output, 1 = Input

}

//void NeoPixel_PWM4_Initialize(void) {
//
//    // first, map PWM4 to RD8
//
//    // Unlock PPS registers 
//    __builtin_write_RPCON(0x0000);
//
//    RPOR4bits.RP41R = PWM4L_OFC;
//
//    // Lock PPS registers
//    __builtin_write_RPCON(0x0800);
//
//    // Configure RD8 as a Digital Output (0 = Output)
//    TRISDbits.TRISD8 = 0;
//
//    // then setup the pWM module
//
//    // Reset control registers for a clean start
//    PG4CONL = 0x0000;
//    PG4CONH = 0x0000;
//    PG4IOCONH = 0x0000;
//
//    // Control Setup
//    PG4CONLbits.CLKSEL = 0b01; // CLKSEL: Select Primary Clock (Fp)
//    PG4CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode
//    PG4CONHbits.MDCSEL = 0; // MDCS: Use PG4DC register for duty cycle value
//
//    //Output Pin Enable
//    PG4IOCONHbits.PENL = 1; // PENL: Enable PWM4L (Low-side) output pin
//
//    //Period and Duty Cycle Setting
//    PG4PER = freq_to_pgxper(4000); 
//    PG4DC = PG4PER / 2;
//
//    // --- Enable PWM ---
//    PG4CONLbits.ON = 1; // Turn ON PWM Generator 4
//}

