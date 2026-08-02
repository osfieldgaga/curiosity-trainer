/* **************************************************************************
*
*  @Company
*    Kettering University
*
*  @File Name
*    io_expander.h
*
*  @Summary
*
*  @Description
*
*  @Author
*    Osfield Gaga
*
*  @Created on
* 
*  @Last modification
************************************************************************** */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IOEXPANDER_H
#define	IOEXPANDER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include <stdint.h>
#include <libpic30.h>

static uint8_t fb[1024];


uint8_t io_read();
void io_cmd(uint8_t reg, uint8_t data);
void init_IO_expander(void);



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* IOEXPANDER_H */

