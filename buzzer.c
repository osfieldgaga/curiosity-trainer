/*
 * File:   buzzer.c
 * Author: Osfield
 *
 * Created on December 28, 2025, 3:59 PM
 */


#include "xc.h"
#include "config.h"
#include "buzzer.h"
#include "pwm.h"
#include <libpic30.h>

void Buzzer_Init() {
    
    // by default, initializing PWM4 routes it to the buzzer
    PWM4_Initialize();
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

void play_note(int freq) {
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
        play_note(sequence[i]);
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
