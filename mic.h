/* **************************************************************************
*
*  @Company
*    Kettering University
*
*  @File Name
*    mic.h
*
*  @Summary
*    Microphone driver: captures frames of audio samples with the ADC
*
*  @Description
*    The microphone is on RB8 (AN10), shared with the ultrasonic sensor echo
*    through a selector switch: set the selector to the microphone before
*    using this driver.
*
*    Samples are taken by the shared ADC core, started by a software trigger
*    at a fixed rate timed by SCCP2 (used as a timer). A frame of samples is
*    captured in a buffer, then sent over UART with mic_send_frame() for the
*    tools/mic_viewer.py and tools/mic_viewer.m scripts.
*
*    Frame format (all values little endian, 10 + 2 * len bytes):
*      0xA5 0x5A | seq (uint16) | len (uint16) | sample rate in Hz (uint16)
*      | len samples (uint16, 0 to 4095) | checksum (uint16, sum of samples)
*
*  @Author
*    Osfield Gaga
*
*  @Created on
*      November 17, 2026
************************************************************************** */

#ifndef MIC_H
#define	MIC_H

#include <xc.h>
#include <stdint.h>

#define MIC_SAMPLE_RATE 8000    // Hz, from 245 Hz (16-bit SCCP2 period) to 65535 Hz (uint16_t)
#define MIC_FRAME_LEN   512     // samples per frame, 64 ms at 8 kHz
// 9600 baud is far too slow for audio frames. 115200 is a standard rate that
// MATLAB accepts on every platform (~6.5 frames/s). 250000 is exact at 16 MHz
// and faster (~9.5 frames/s) but MATLAB on Linux rejects it. The debugger's USB
// bridge drops bytes at 500000 and above
#define MIC_UART_BAUD   115200

#define MIC_SYNC1 0xA5
#define MIC_SYNC2 0x5A

void mic_init(uint16_t sample_rate); // RB8 analog input, ADC and sample timer
uint16_t mic_read(void); // one conversion, 0 to 4095 (0 V to 3.3 V)
void mic_capture(uint16_t *buf, uint16_t len); // len samples at the sample rate (blocking)
void mic_send_frame(const uint16_t *buf, uint16_t len, uint16_t seq); // send a frame over UART

#endif	/* MIC_H */
