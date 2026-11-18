/* 
 * File:   neopixels.h
 * Author: Osfield
 *
 * Created on December 17, 2025, 1:35 PM
 */

#include "xc.h"
#include "config.h"
//#include <libpic30.h>   // for Nop()

#ifndef NEOPIXELS_H
#define	NEOPIXELS_H

#define NEO_PIN LATBbits.LATB9
#define NEO_TRIS TRISBbits.TRISB9
#define NEO_ANSEL ANSELBbits.ANSELB9



/**
 * @brief Sends a single bit of data using a custom pulse-width modulation scheme on pin RE0.
 *
 * This macro generates a pulse on PORTEbits.RE0.  The pulse starts with a short high pulse,
 * followed by a level determined by the input 'b', and ends with a low period.  The timing
 * of the pulse is controlled by NOP() instructions, making it suitable for bit-banging
 * a custom single-wire communication protocol.  The do...while(0) loop ensures the macro
 * behaves as a single statement.
 *
 * @param b The data bit to send (0 or 1).  This value determines the state of the pin
 *          after the initial high pulse.  If b is 1, the pin remains high for a longer
 *          duration. If b is 0, the pin goes low sooner.
 *
 * @note  The exact timing and protocol are implementation-specific.  The number of NOP()
 *        instructions should be carefully calibrated to match the receiver's expectations.
 *        This macro assumes that PORTE and its associated registers are properly configured
 *        for digital output.
 *
 * Example Usage:
 *   send1(1);  // Send a '1' bit (longer high pulse)
 *   send1(0);  // Send a '0' bit (shorter high pulse)
 */
//#define send1(b)  do{NEO_PIN=1; Nop(); NEO_PIN=b; Nop(); NEO_PIN=0; Nop(); Nop(); Nop();} while(0);

// Timing at Fcy = 16 MHz (62.5 ns per instruction cycle):
//   0 bit: high 4 cycles = 250 ns,  then low  >= 16 cycles
//   1 bit: high 10 cycles = 625 ns, then low  >= 10 cycles
#define send0() do{ NEO_PIN=1; Nop(); Nop(); Nop(); NEO_PIN=0; Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); } while(0)
#define send1() do{ NEO_PIN=1; Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); NEO_PIN=0; Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); } while(0)

void NEO_Init();
void sendByte(uint8_t b);
void sendRGB (uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void sendRGBMultiple(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, int count);
void performPattern(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, int maxCount);

#ifdef	__cplusplus
extern "C" {
#endif
    




#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIXELS_H */

