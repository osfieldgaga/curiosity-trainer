/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    sccp.h
 *
 *  @Summary
 *    Defines function prototypes for the SCCP module
 *
 *  @Description
 *    The functions used for interfacing the SCCP module and the function prototypes
 *    are defined in this header file.
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


#ifndef SCCP_H
#define	SCCP_H

#include <xc.h> // include processor files - each processor file is guarded.  

void SCCP_Init();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* SCCP_H */

