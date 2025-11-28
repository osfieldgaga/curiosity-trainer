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

    // first, map PWM4 to RD8

    // Unlock PPS registers 
    __builtin_write_RPCON(0x0000);

    // RD8 is RP72, controlled by RPOR15[13:8]
    RPOR15bits.RP72R = PWM4L_OFC;

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
    // Fp = 4 MHz. PG4PER = (4,000,000 / 100,000) - 1 = 39
    PG4PER = 15286; // PERIOD: Set the PWM period for 100 kHz
    PG4DC = PG4PER / 2; // DUTY CYCLE: Set to 50% (20 out of 40 total steps)

    // --- Enable PWM ---
    PG4CONLbits.ON = 1; // Turn ON PWM Generator 4
}




/*******************************************************************************
 * Music Generator 
 * 
 * This section is just a proof of concept, showing it's possible to set up the 
 * PWM to achieve pretty cool stuff
 * 
 * A sequence of note can be played, one array contains the notes sns rests, another
 * contains their durations. The note/rest and its corresponding duration are 
 * at the same index in the two arrays
 ******************************************************************************/

// the sequence to play
const uint16_t megalovania[64] = {
    D4, REST, D4, D5, A4, REST, G4_SHARP, REST, G4, REST, F4, REST, D4, F4, G4,

    C4, REST, C4, D5, A4, REST, G4_SHARP, REST, G4, REST, F4, REST, D4, F4, G4,

    B3, REST, B3, D5, A4, REST, G4_SHARP, REST, G4, REST, F4, REST, D4, F4, G4,

    A3_SHARP, REST, A3_SHARP, D5, A4, REST, G4_SHARP, REST, G4, REST, F4, REST, D4, F4, G4,
};

// the corresponding durations
const float megalovania_durations[64] = {
    // Measure 1 & 2 (Pattern 1: D D D5 A4 G# G F D F G)
    EIGTH, EIGTH, QUART, HALF, HALF, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART,
    // Measure 3 & 4 (Pattern 2: C C D5 A4 G# G F D F G)
    EIGTH, EIGTH, QUART, HALF, HALF, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART,
    // Measure 5 & 6 (Pattern 3: B3 B3 D5 A4 G# G F D F G)
    EIGTH, EIGTH, QUART, HALF, HALF, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART,
    // Measure 7 & 8 (Pattern 4: A# A# D5 A4 G# G F D F G)
    EIGTH, EIGTH, QUART, HALF, HALF, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART, QUART,
};

void play_music() {
    play_sequence(megalovania, megalovania_durations, 120.0f, 64);
}

// Convert a desired duration to the corresponding PPGxPER value

int freq_to_pgxper(int freq) {
    return (FP / freq) - 1;
}

// Notes are played on PWM4, either a note is played or a rest
// notes turn on the PWM module and set the appropriate PG4PER
// Rests turn off the PWM module

void play_note_pwm4(int freq) {
    if (freq == REST) {
        // Handle the rest: Turn the PWM generator OFF
        PG4CONLbits.ON = 0;
    } else {
        // Handle a regular note: Ensure PWM is ON and set period/DC
        PG4CONLbits.ON = 1; // Ensure the generator is ON
        while (PG4STATbits.UPDATE == 1);
        PG4PER = freq_to_pgxper(freq);
        PG4DC = PG4PER / 2;
        PG4STATbits.UPDREQ = 1;
    }
}

// playing a sequence is playing the specific note/rest ad delaying for the 
//  desired duration

void play_sequence(const uint16_t* sequence, const float* durations, uint16_t bpm, uint16_t sequence_length) {

    for (int i = 0; i < sequence_length; i++) {
        play_note_pwm4(sequence[i]);
        __delay_ms(calculate_duration(bpm, durations[i]));
    }
}

// to determine the duration of a note, you calculate how beat in a ms, then multply
// that by the number of beats of the desired note

uint16_t calculate_duration(uint16_t bpm, float note_type) {
    float ms_per_beat = (60.0 / bpm) * 1000.0;
    float total_delay_ms = ms_per_beat * note_type;
    return (uint16_t) total_delay_ms;
}