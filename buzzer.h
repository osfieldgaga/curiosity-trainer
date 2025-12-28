/* 
 * File:   buzzer.h
 * Author: Osfield
 *
 * Created on December 28, 2025, 4:00 PM
 */

#ifndef BUZZER_H
#define	BUZZER_H

#ifdef	__cplusplus
extern "C" {
#endif

void Buzzer_Init();
int freq_to_pgxper(int freq);
void play_sequence(const uint16_t* sequence, const float* durations, uint16_t bpm, uint16_t sequence_length);
void play_note(int freq);
void play_music();

uint16_t calculate_duration(uint16_t bpm, float note_type);

/*******************************************************************************
 * Music Generator Setup
 * 
 * This section sets up the PWM frequencies to generate musical notes from C3 to C6
 * on a piano. 
 * 
 * The equation to determine the required PGxPER value can be found on page 56 this guide
 * https://ww1.microchip.com/downloads/aemDocuments/documents/MCU16/ProductDocuments/ReferenceManuals/dsPIC33-PIC24-FRM-High-Resolution-PWM-with-Fine-Edge-Placement-DS70005320.pdf
 * 
 * These values assume Fp = 4MHz, meaning the main clock is FRC @ 8MHz
 ******************************************************************************/


// --- Note Frequency Definitions (Hz) for C3 to C6 ---

#define C3      130.81
#define C3_SHARP 138.59
#define D3      146.83
#define D3_SHARP 155.56
#define E3      164.81
#define F3      174.61
#define F3_SHARP 185.00
#define G3      196.00
#define G3_SHARP 207.65
#define A3      220.00
#define A3_SHARP 233.08
#define B3      246.94

#define C4      261.63 // Middle C
#define C4_SHARP 277.18
#define D4      293.66
#define D4_SHARP 311.13
#define E4      329.63
#define F4      349.23
#define F4_SHARP 369.99
#define G4      392.00
#define G4_SHARP 415.30
#define A4      440.00 // Standard tuning pitch
#define A4_SHARP 466.16
#define B4      493.88

#define C5      523.25
#define C5_SHARP 554.37
#define D5      587.33
#define D5_SHARP 622.25
#define E5      659.25
#define F5      698.46
#define F5_SHARP 739.99
#define G5      783.99
#define G5_SHARP 830.61
#define A5      880.00
#define A5_SHARP 932.33
#define B5      987.77

#define C6      1046.50

#define REST 0

// The durations for each note or rest
#define FULL 4.0f
#define DOUBLE FULL/2
#define SINGLE DOUBLE/2
#define HALF SINGLE/2
#define QUART HALF/2
#define EIGTH QUART/2
#define SIXTEENTH EIGTH/2

#ifdef	__cplusplus
}
#endif

#endif	/* BUZZER_H */

