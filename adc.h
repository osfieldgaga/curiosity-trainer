/* **************************************************************************
*
*  @Company
*    Kettering University
*
*  @File Name
*    adc.h
*
*  @Summary
*    Defines function prototypes for the ADC Core module
*
*  @Description
*    The functions used for interfacing the ADC Core and the function prototypes
*    are defined in this header file.
*
*  @Author
*    Osfield Gaga & Jeremy Gooch
*
*  @Created on
*      November 27, 2025, 9:35 PM
* 
*  @Last modification
*       December 08, 2025, 11:50 PM
/* ************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "config.h"
#include <libpic30.h>


int ADC_Init(void);
int ADC_Core_Read();
void ADC_Set_AN9();


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ADC_H */

