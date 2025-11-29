/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "stdint.h"

// Lata

#define LED0 LATCbits.LATC6
#define LED1 LATCbits.LATC7
#define LED2 LATCbits.LATC12
#define LED3 LATCbits.LATC13
#define LED4 LATDbits.LATD10
#define LED5 LATAbits.LATA0
#define LED6 LATBbits.LATB2
#define LED7 LATAbits.LATA1

#define RGB_R LATBbits.LATB10
#define RGB_G LATDbits.LATD8
#define RGB_B LATDbits.LATD1



// Tris

#define LED0_TRIS TRISCbits.TRISC6
#define LED1_TRIS TRISCbits.TRISC7
#define LED2_TRIS TRISCbits.TRISC12
#define LED3_TRIS TRISCbits.TRISC13
#define LED4_TRIS TRISDbits.TRISD10
#define LED5_TRIS TRISAbits.TRISA0
#define LED6_TRIS TRISBbits.TRISB2
#define LED7_TRIS TRISAbits.TRISA1

#define RGB_R_TRIS TRISBbits.TRISB10
#define RGB_G_TRIS TRISDbits.TRISD8
#define RGB_B_TRIS TRISDbits.TRISD1



// Ansell

#define LED0_ANSEL ANSELCbits.ANSELC6
#define LED1_ANSEL ANSELCbits.ANSELC7
#define LED2_ANSEL ANSELCbits.ANSELC12
#define LED3_ANSEL ANSELCbits.ANSELC13
#define LED4_ANSEL ANSELDbits.ANSELD10
#define LED5_ANSEL ANSELAbits.ANSELA0
#define LED6_ANSEL ANSELBbits.ANSELB2
#define LED7_ANSEL ANSELAbits.ANSELA1

#define RGB_R_ANSEL ANSELBbits.ANSELB10
#define RGB_G_ANSEL ANSELDbits.ANSELD8
#define RGB_B_ANSEL ANSELDbits.ANSELD1


#define LED_ON 0
#define LED_OFF 1

void turn_off_leds();
void set_led_state(uint8_t index, uint8_t state);
void set_all_state(uint8_t state);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

