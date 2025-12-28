#include <xc.h>
#include <stdint.h>
#include "pwm.h"

void PWM1_Initialize(void) {
    // Reset control registers for a clean start
    PG1CONL = 0x0000;
    PG1CONH = 0x0000;
    PG4IOCONH = 0x0000;

    // --- Control Setup ---
    PG1CONLbits.CLKSEL = 0b01; // CLKSEL: Select Primary Clock (Fp)
    PG1CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode
    PG1CONHbits.MDCSEL = 0; // MDCS: Use PG4DC register for duty cycle value

    // --- Output Pin Enable ---
    PG1IOCONHbits.PENL = 1; // PENL: Enable PWM4L (Low-side) output pin

    // --- Period and Duty Cycle Setting ---
    // Fp = 4 MHz. PG4PER = (4,000,000 / 100,000) - 1 = 39
    PG1PER = 10000; // PERIOD: Set the PWM period for 100 kHz
    PG1DC = 500; // DUTY CYCLE: Set to 50% (20 out of 40 total steps)

    // --- Enable PWM ---
    PG1CONLbits.ON = 1; // Turn ON PWM Generator 4
}

void PWM2_Initialize(void) {
    // Reset control registers for a clean start
    PG2CONL = 0x0000;
    PG2CONH = 0x0000;
    PG2IOCONH = 0x0000;

    // --- Control Setup ---
    PG2CONLbits.CLKSEL = 0b01; // CLKSEL: Select Peripheral Clock (Fp)

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
    PG2CONHbits.MDCSEL = 0; // MDCS: Use PG4DC register for duty cycle value

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
    // Fp = 4 MHz. PG4PER = (4,000,000 / 100,000) - 1 = 39
    PG2PER = 15286; // PERIOD: Set the PWM period for 100 kHz
    PG2DC = PG2PER / 2; // DUTY CYCLE: Set to 50% (20 out of 40 total steps)

    // --- Enable PWM ---
    PG2CONLbits.ON = 1; // Turn ON PWM Generator 4
}

void PWM3_Initialize(void) {
    // Reset control registers for a clean start
    PG3CONL = 0x0000;
    PG3CONH = 0x0000;
    PG3IOCONH = 0x0000;

    // --- Control Setup ---
    PG3CONLbits.CLKSEL = 0b01; // CLKSEL: Select Primary Clock (Fp)
    PG3CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode
    PG3CONHbits.MDCSEL = 0; // MDCS: Use PG4DC register for duty cycle value

    // --- Output Pin Enable ---
    PG3IOCONHbits.PENL = 1; // PENL: Enable PWM4L (Low-side) output pin

    // --- Period and Duty Cycle Setting ---
    // Fp = 4 MHz. PG4PER = (4,000,000 / 100,000) - 1 = 39
    PG3PER = 15286; // PERIOD: Set the PWM period for 100 kHz
    PG3DC = 500; // DUTY CYCLE: Set to 50% (20 out of 40 total steps)

    // --- Enable PWM ---
    PG4CONLbits.ON = 1; // Turn ON PWM Generator 4
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
    PG4CONLbits.CLKSEL = 0b01; // CLKSEL: Select Primary Clock (Fp)
    PG4CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode
    PG4CONHbits.MDCSEL = 0; // MDCS: Use PG4DC register for duty cycle value

    // --- Output Pin Enable ---
    PG4IOCONHbits.PENL = 1; // PENL: Enable PWM4L (Low-side) output pin

    // --- Period and Duty Cycle Setting ---
    PG4PER = 15286; 
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

void NeoPixel_PWM4_Initialize(void) {

    // first, map PWM4 to RD8

    // Unlock PPS registers 
    __builtin_write_RPCON(0x0000);

    RPOR4bits.RP41R = PWM4L_OFC;

    // Lock PPS registers
    __builtin_write_RPCON(0x0800);

    // Configure RD8 as a Digital Output (0 = Output)
    TRISDbits.TRISD8 = 0;

    // then setup the pWM module

    // Reset control registers for a clean start
    PG4CONL = 0x0000;
    PG4CONH = 0x0000;
    PG4IOCONH = 0x0000;

    // Control Setup
    PG4CONLbits.CLKSEL = 0b01; // CLKSEL: Select Primary Clock (Fp)
    PG4CONLbits.MODSEL = 0b000; // MODSEL: Independent Edge PWM Mode
    PG4CONHbits.MDCSEL = 0; // MDCS: Use PG4DC register for duty cycle value

    //Output Pin Enable
    PG4IOCONHbits.PENL = 1; // PENL: Enable PWM4L (Low-side) output pin

    //Period and Duty Cycle Setting
    PG4PER = freq_to_pgxper(4000); 
    PG4DC = PG4PER / 2;

    // --- Enable PWM ---
    PG4CONLbits.ON = 1; // Turn ON PWM Generator 4
}

