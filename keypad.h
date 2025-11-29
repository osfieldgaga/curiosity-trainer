/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef KEYPAD_H
#define	KEYPAD_H

#include <xc.h> // include processor files - each processor file is guarded. 


//#define COL4_TRIS TRISBbits.TRISB9
#define COL3_TRIS TRISAbits.TRISA4
#define COL2_TRIS TRISCbits.TRISC0
#define COL1_TRIS TRISCbits.TRISC1

#define ROW4_TRIS TRISCbits.TRISC2
#define ROW3_TRIS TRISCbits.TRISC3
#define ROW2_TRIS TRISCbits.TRISC4
#define ROW1_TRIS TRISCbits.TRISC5

//#define COL4 LATBbits.LATB9
#define COL3 LATAbits.LATA4
#define COL2 LATCbits.LATC0
#define COL1 LATCbits.LATC1

#define ROW4 PORTCbits.RC2
#define ROW3 PORTCbits.RC3
#define ROW2 PORTCbits.RC4
#define ROW1 PORTCbits.RC5

//#define COL4_ANSEL ANSELBbits.ANSELB9
#define COL3_ANSEL ANSELAbits.ANSELA4
#define COL2_ANSEL ANSELCbits.ANSELC0
#define COL1_ANSEL ANSELCbits.ANSELC1

#define ROW4_ANSEL ANSELCbits.ANSELC2
#define ROW3_ANSEL ANSELCbits.ANSELC3
#define ROW2_ANSEL ANSELCbits.ANSELC4
#define ROW1_ANSEL ANSELCbits.ANSELC5

#define COL_ON 0
#define COL_OFF 1

#define ROW_ON 0
#define ROW_OFF 1

#define KEYPAD_OUTPUT_CHAR 0
#define KEYPAD_OUTPUT_DECIMAL 1

void init_keypad();
unsigned char readKey(uint8_t outputMode);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* KEYPAD_H */

