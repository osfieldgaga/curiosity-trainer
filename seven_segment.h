 /* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    seven_segment.h
 *
 *  @Summary
 *    Defines function prototypes for the 7 segment display
 *
 *  @Description
 *    The functions used for interfacing the 7 segment display and the function prototypes
 *    are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 23, 2025, 11:36 PM
 * 
 *  @Last modification
 *      December 09, 2025, 5:25 PM
/* ************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SEVEN_SEGMENT_H
#define	SEVEN_SEGMENT_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "config.h"
#include <stdint.h>


// define anodes

//#define DIG4_TRIS TRISCbits.TRISC6
//#define DIG3_TRIS TRISCbits.TRISC7
//#define DIG2_TRIS TRISCbits.TRISC12
//#define DIG1_TRIS TRISCbits.TRISC13
//
//#define DIG4 LATCbits.LATC6
//#define DIG3 LATCbits.LATC7
//#define DIG2 LATCbits.LATC12
//#define DIG1 LATCbits.LATC13
//
//
//// current config does not have these connected on analog pins
//
//#define DIG4_ANSEL ANSELCbits.ANSELC6
//#define DIG3_ANSEL ANSELCbits.ANSELC7
//#define DIG2_ANSEL ANSELCbits.ANSELC12
//#define DIG1_ANSEL ANSELCbits.ANSELC13
//
//#define ANODE_ON 1
//#define ANODE_OFF 0
//
//// define segments 
//
//#define LED_A_TRIS TRISDbits.TRISD10 //
//#define LED_B_TRIS TRISAbits.TRISA0 //
//#define LED_C_TRIS TRISBbits.TRISB2 //
//#define LED_D_TRIS TRISAbits.TRISA3 //
//#define LED_E_TRIS TRISBbits.TRISB10
//#define LED_F_TRIS TRISDbits.TRISD8
//#define LED_G_TRIS TRISDbits.TRISD1
//#define LED_DP_TRIS TRISAbits.TRISA1
//
//#define LED_A LATDbits.LATD10 //
//#define LED_B LATAbits.LATA0 //
//#define LED_C LATBbits.LATB2 //
//#define LED_D LATAbits.LATA3 //
//#define LED_E LATBbits.LATB10
//#define LED_F LATDbits.LATD8
//#define LED_G LATDbits.LATD1
//#define LED_DP LATAbits.LATA1
//
//
//// define analog select bits
//
//#define LED_A_ANSEL ANSELDbits.ANSELD10 //
//#define LED_B_ANSEL ANSELAbits.ANSELA0 //
//#define LED_C_ANSEL ANSELBbits.ANSELB2 //
//#define LED_D_ANSEL ANSELAbits.ANSELA3 //
//#define LED_E_ANSEL ANSELBbits.ANSELB10
//#define LED_F_ANSEL ANSELDbits.ANSELD8
//#define LED_G_ANSEL ANSELDbits.ANSELD1
//#define LED_DP_ANSEL ANSELAbits.ANSELA1

#define ON 0
#define OFF 1

#define DP_ON 0
#define DP_OFF 1


void init_seven_seg();
void print_seven_seg(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

void refresh_seven_seg();
void seven_seg_display_off();

void print_decimal_point(uint8_t dp0, uint8_t dp1, uint8_t dp2, uint8_t dp3);



#define DIG4_TRIS TRISDbits.TRISD1 //
#define DIG3_TRIS TRISDbits.TRISD8 //
#define DIG2_TRIS TRISBbits.TRISB10 //
#define DIG1_TRIS TRISBbits.TRISB9 //

#define DIG4 LATDbits.LATD1 //
#define DIG3 LATDbits.LATD8 //
#define DIG2 LATBbits.LATB10 //
#define DIG1 LATBbits.LATB9 //


// current config does not have these connected on analog pins

//#define DIG4_ANSEL ANSELDbits.ANSELD1
//#define DIG3_ANSEL ANSELDbits.ANSELD8
//#define DIG2_ANSEL ANSELBbits.ANSELB10
#define DIG1_ANSEL ANSELBbits.ANSELB9

// L = RA1

#define ANODE_ON 0
#define ANODE_OFF 1

// define segments 

#define LED_A_TRIS TRISCbits.TRISC6 
#define LED_B_TRIS TRISCbits.TRISC7 
#define LED_C_TRIS TRISCbits.TRISC12 
#define LED_D_TRIS TRISCbits.TRISC13 
#define LED_E_TRIS TRISDbits.TRISD10
#define LED_F_TRIS TRISAbits.TRISA0
#define LED_G_TRIS TRISBbits.TRISB2
#define LED_DP_TRIS TRISAbits.TRISA3

#define LED_A LATCbits.LATC6 
#define LED_B LATCbits.LATC7
#define LED_C LATCbits.LATC12 
#define LED_D LATCbits.LATC13
#define LED_E LATDbits.LATD10
#define LED_F LATAbits.LATA0
#define LED_G LATBbits.LATB2
#define LED_DP LATAbits.LATA3


// define analog select bits

#define LED_A_ANSEL ANSELCbits.ANSELC6
#define LED_B_ANSEL ANSELCbits.ANSELC7
//#define LED_C_ANSEL ANSELCbits.ANSELC12
//#define LED_D_ANSEL ANSELCbits.ANSELC13
#define LED_E_ANSEL ANSELDbits.ANSELD10
#define LED_F_ANSEL ANSELAbits.ANSELA0
#define LED_G_ANSEL ANSELBbits.ANSELB2
#define LED_DP_ANSEL ANSELAbits.ANSELA3



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* SEVEN_SEGMENT_H */

