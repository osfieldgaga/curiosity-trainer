/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    led.h
 *
 *  @Summary
 *    Defines function prototypes for the photoresistor
 *
 *  @Description
 *    The functions used for interfacing the photoresistor and the function 
 *    prototypes are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 27, 2025, 9:38 PM
 * 
 *  @Last modification
 *      December 09, 2025, 5:22 PM
/* ************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PHOTORESISTOR_H
#define	PHOTORESISTOR_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "config.h"
#include <libpic30.h>

void LDR_Init(void);
int  read_ldr (float min, float max);

#define LDR PORTAbits.RA2
#define LDR_TRIS TRISAbits.TRISA2
#define LDR_ANSEL ANSELAbits.ANSELA2




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* PHOTORESISTOR_H */

