/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    switches.h
 *
 *  @Summary
 *    Defines function prototypes for the slide switches
 *
 *  @Description
 *    The functions used for interfacing the slide switches and the function prototypes
 *    are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      December 3, 2025, 12:25 PM
 * 
 *  @Last modification
 *      December 9, 2025, 5:27 PM
/* ************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SWITCHES_H
#define	SWITCHES_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "config.h"

#define SW1 PORTCbits.RC5
#define SW2 PORTCbits.RC4
#define SW3 PORTCbits.RC3
#define SW4 PORTCbits.RC2
#define SW5 PORTBbits.RB15
#define SW6 PORTBbits.RB14
#define SW7 PORTBbits.RB13
#define SW8 PORTBbits.RB12

#define SW1_TRIS TRISCbits.TRISC5
#define SW2_TRIS TRISCbits.TRISC4
#define SW3_TRIS TRISCbits.TRISC3
#define SW4_TRIS TRISCbits.TRISC2
#define SW5_TRIS TRISBbits.TRISB15
#define SW6_TRIS TRISBbits.TRISB14
#define SW7_TRIS TRISBbits.TRISB13
#define SW8_TRIS TRISBbits.TRISB12

//#define SW1_ANSEL ANSELCbits.ANSELC5
//#define SW2_ANSEL ANSELCbits.ANSELC4
#define SW3_ANSEL ANSELCbits.ANSELC3
#define SW4_ANSEL ANSELCbits.ANSELC2
//#define SW5_ANSEL ANSELBbits.ANSELB15
//#define SW6_ANSEL ANSELBbits.ANSELB14
//#define SW7_ANSEL ANSELBbits.ANSELB13
//#define SW8_ANSEL ANSELBbits.ANSELB12

#define SW_ON 0
#define SW_OFF 1

void SW_Init ();
uint8_t read_switch (uint8_t index);
uint8_t read_switches();


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* SWITCHES_H */

