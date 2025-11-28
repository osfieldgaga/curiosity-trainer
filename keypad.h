/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

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


#define COL4_TRIS TRISBbits.TRISB9
#define COL3_TRIS TRISAbits.TRISA4
#define COL2_TRIS TRISCbits.TRISC0
#define COL1_TRIS TRISCbits.TRISC1

#define ROW4_TRIS TRISCbits.TRISC2
#define ROW3_TRIS TRISCbits.TRISC3
#define ROW2_TRIS TRISCbits.TRISC4
#define ROW1_TRIS TRISCbits.TRISC5

#define COL4 LATBbits.LATB9
#define COL3 LATAbits.LATA4
#define COL2 LATCbits.LATC0
#define COL1 LATCbits.LATC1

#define ROW4 PORTCbits.RC2
#define ROW3 PORTCbits.RC3
#define ROW2 PORTCbits.RC4
#define ROW1 PORTCbits.RC5

#define COL4_ANSEL ANSELBbits.ANSELB9
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

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* KEYPAD_H */

