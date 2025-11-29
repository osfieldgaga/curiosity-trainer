/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PHOTORESISTOR_H
#define	PHOTORESISTOR_H

#include <xc.h> // include processor files - each processor file is guarded.  

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

