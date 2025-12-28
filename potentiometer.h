/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    potentiometer.h
 *
 *  @Summary
 *    Defines function prototypes for the potentiometer
 *
 *  @Description
 *    The functions used for interfacing the potentiometer and the function 
 *    prototypes are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 23, 2025, 6:04 PM
 * 
 *  @Last modification
 *      December 09, 2025, 5:23 PM
/* ************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef POTENTIOMETER_H
#define	POTENTIOMETER_H

#include <xc.h> // include processor files - each processor file is guarded.  


void POT_Init(void);
int  read_pot (float min, float max);

#define POT PORTAbits.RA2
#define POT_TRIS TRISAbits.TRISA2
#define POT_ANSEL ANSELAbits.ANSELA2



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* POTENTIOMETER_H */

