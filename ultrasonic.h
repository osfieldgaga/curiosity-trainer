/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    sccp.h
 *
 *  @Summary
 *    Defines function prototypes for the ultrasonic proximity sensor
 *
 *  @Description
 *    The functions used for interfacing the ultrasonic proximity sensor and the
 *    function prototypes are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      December 10, 2025, 3:02 PM
 * 
 *  @Last modification
 *      December 10, 2025, 9:22 PM
/* ************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ULTRASONIC_H
#define	ULTRASONIC_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define TRIGGER LATBbits.LATB7
#define ECHO PORTBbits.RB8

#define TRIGGER_TRIS TRISBbits.TRISB7
#define ECHO_TRIS TRISBbits.TRISB8

#define TRIGGER_ANSEL ANSELBbits.ANSELB7
#define ECHO_ANSEL ANSELBbits.ANSELB8

int sense_distance ();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ULTRASONIC_H */

