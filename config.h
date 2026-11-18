/* **************************************************************************
*
*  @Company
*    Kettering University
*
*  @File Name
*    config.h
*
*  @Summary
*    Defines board configuration
*
*  @Description
*    The macros used throughout the different drivers, as well as the period of 
*    Timer1 is defined in this file.
*
*  @Author
*    Osfield Gaga & Jeremy Gooch
*
*  @Created on
*      November 22, 2025, 4:06 PM
* 
*  @Last modification
*       December 09, 2025, 12:35 AM
* ************************************************************************** */

#ifndef CONFIG_H
#define	CONFIG_H
#include "xc.h"

// Instruction clock: FRC (8 MHz) + PLL set up in main.c gives Fpllo = 64 MHz,
// Fosc = Fpllo / 2 = 32 MHz and Fcy = Fp = Fosc / 2 = 16 MHz. The neopixels bit
// timing (Nop() counts in neopixels.h) is tuned for this clock, so keep it.
// FCY is used by __delay_ms(), __delay_us() and to compute the UART baud rate
#define FCY 16000000UL

/******************************************************************************
 *                                  Clocks
 * 
 ******************************************************************************/

#define FOSC 32000000UL // FRC + PLL (main.c), also clocks the PWM module

/******************************************************************************
 *                     Timer 1 Default configuration
 * 
 ******************************************************************************/

#define T1_TIME_MS 1000 // tFlag period 
#define T1_PERIOD 1999 // 1 ms at Fp = 16 MHz (FRC + PLL set up in main.c) with the 1:8 prescaler

// a center button press must be stable for this long before it ends the example
#define BTN_DEBOUNCE_MS 20

extern volatile uint16_t msCount; // incremented every 1ms, reset once equals to T1_TIME_MS
extern volatile uint8_t tFlag; // flag set every T1_TIME_MS ms, must be explicitely reset by the user
extern volatile uint16_t msTicks; // incremented every 1ms, never reset (wraps at 65535)
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

