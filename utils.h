/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    utils.h
 *
 *  @Summary
 *    Defines function prototypes for the utilities
 *
 *  @Description
 *    The functions used for the utilities and the function prototypes
 *    are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 22, 2025, 4:02 PM
 * 
 *  @Last modification
 *      December 9, 2025, 7:10 PM
/* ************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UTILS_H
#define	UTILS_H

#include <xc.h> // include processor files - each processor file is guarded.  


extern uint16_t delayCounterMs;
extern uint8_t enDelay;


void msDelay(int ms);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* UTILS_H */

