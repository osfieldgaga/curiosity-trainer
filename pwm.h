/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWM_H
#define	PWM_H

// FCY must be set before including libpic30.h, it is defined in config.h
#include "config.h"
#include <libpic30.h>
#include <xc.h> // include processor files - each processor file is guarded.  


// --- Global Constants ---

// PWM4 (buzzer) counter clock: Fosc / 16 = 32 MHz / 16, see PWM4_Initialize()
#define FP 2000000UL

#define PWM1_PERIOD
#define PWM1_DC

#define PWM2_PERIOD
#define PWM2_DC

#define PWM3_PERIOD
#define PWM3_DC

#define PWM4_PERIOD
#define PWM4_DC

// PWM4L Output Function Code (OFC)
// refer to the PPS section of the documentation for more info
#define PWM4L_OFC 35 

// --- Function Prototypes ---
void PWM1_Initialize(void);
void PWM2_Initialize(void);
void PWM3_Initialize(void);
void PWM4_Initialize(void);

void PWM4_Disable(void);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* PWM_H */

