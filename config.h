/*
 * File:   config.h
 * Author: Osfield
 *
 * Created on November 22, 2025, 4:06 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H
#include "xc.h"

/******************************************************************************
 *                                  Clocks
 * 
 ******************************************************************************/

#define CPU_FREQ 8000000 // using FRC by default
#define FP CPU_FREQ / 2

/******************************************************************************
 *                     Timer 1 Default configuration
 * 
 ******************************************************************************/

#define T1_TIME_MS 1000 // tFlag period 
#define T1_PERIOD 500 // corresponds to 1 ms at Fosc = 8Mhz, and Fpb = Fosc/2

extern uint16_t msCount; // incremented every 1ms, reset once equals to T1_TIME_MS
extern uint8_t tFlag; // flag set every T1_TIME_MS ms, must be explicitely reset by the user
void init_T1(); // sets up timer 1, enables T1 and interrupts

/******************************************************************************
 *                    Tristate and Analog select macros
 * 
 ******************************************************************************/

// Tri-state directions
#define TRIS_OUTPUT 0
#define TRIS_INPUT 1

// Analog select type
#define ANSEL_DIGITAL 0
#define ANSEL_ANALOG 1

#define TRUE 1
#define FALSE 0


/******************************************************************************
 *                     Seven segment configuration
 * 
 ******************************************************************************/
extern uint8_t seven_seg_enable; // set to 1 once the seven seg is initialized, used to refresh the display in the T1 ISR



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CONFIG_H */

