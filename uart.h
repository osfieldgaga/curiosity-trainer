

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

#define UART_BAUD 9600

void UartInit(void);               // UART_BAUD (9600) 8N1 on the debugger's virtual COM port
void UartInitBaud(uint32_t baud);  // same, at any baud rate (e.g. 250000 for fast data)
void UartSendBuffer(const uint8_t *pData, uint16_t len); // send raw bytes, zeros included
void UartSendByte(char data);
void UartSendString(char *pData);
uint8_t UartDataAvailable(void); // 1 if a received byte is waiting (also clears a receive overrun)
char UartReadByte(void);          // blocks until a byte is received

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* UART_H */

