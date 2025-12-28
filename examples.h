/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EXAMPLES_H
#define	EXAMPLES_H

#include <xc.h> // include processor files - each processor file is guarded.  

extern volatile uint8_t running; // Global variable


void example1();
void example2();
void example3();
void example4();
void example5();
void example6();
void example7();
void example8();
void example9();
void example10();
void example11();
 void example12();
 void example13(); 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* EXAMPLES_H */

