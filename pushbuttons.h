/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    pushbuttons.h
 *
 *  @Summary
 *    Defines function prototypes for the push buttons
 *
 *  @Description
 *    The functions used for interfacing the push buttons and the function prototypes
 *    are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 28, 2025, 6:40 PM
 * 
 *  @Last modification
 *      December 09, 2025, 5:23 PM
/* ************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PUSHBUTTONS_H
#define	PUSHBUTTONS_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define BTN_C PORTBbits.RB11
#define BTN_C_TRIS TRISBbits.TRISB11

#define BTN_U PORTBbits.RB14
#define BTN_U_TRIS TRISBbits.TRISB14

#define BTN_D PORTBbits.RB12
#define BTN_D_TRIS TRISBbits.TRISB12

#define BTN_L PORTBbits.RB15
#define BTN_L_TRIS TRISBbits.TRISB15

#define BTN_R PORTBbits.RB13
#define BTN_R_TRIS TRISBbits.TRISB13

void BTN_Center_Init();
void BTN_Init();
uint8_t read_center_btn();
uint8_t read_up_btn();
uint8_t read_down_btn();
uint8_t read_left_btn();
uint8_t read_right_btn();


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* PUSHBUTTONS_H */

