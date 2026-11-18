/*
 *
 *  @Company
        Kettering University
 *
 *  @File Name
 *      mic.c
 *
 *  @Summary
 *      Microphone driver: captures frames of audio samples with the ADC
 *
 *  @Description
 *      The microphone is read on AN10 (RB8) by the shared ADC core. Each
 *      conversion is started by the common software trigger when the SCCP2
 *      timer period elapses, so the samples are evenly spaced. See
 *      example17() in examples.c for a demo streaming frames over UART.
 *
 *  @Authors:
 *      Osfield Gaga
 *
 *  @Created on
 *      November 17, 2026
 */

#include "xc.h"
#include "config.h"
#include "uart.h"
#include "mic.h"

static uint16_t micSampleRate = MIC_SAMPLE_RATE;

void mic_init(uint16_t sample_rate) {

    micSampleRate = sample_rate;

    // RB8 is shared with the ultrasonic echo (digital) and is set to digital
    // by init_oled() and init_ultrasonic(), make it an analog input again
    TRISBbits.TRISB8 = TRIS_INPUT;
    ANSELBbits.ANSELB8 = ANSEL_ANALOG;

    // --- Shared ADC core ---
    // same setup as ADC_Init() (adc.c), with a longer sampling time and AN10
    // started by the common software trigger
    ADCON1Lbits.ADON = 0;
    ADCON3Hbits.CLKSEL = 0; // Fp = 16 MHz
    ADCON3Hbits.CLKDIV = 0; // Tcoresrc = 62.5 ns
    ADCON2Lbits.SHRADCS = 0; // Tad = 2 * Tcoresrc = 125 ns
    ADCON2Hbits.SHRSAMC = 6; // sampling time = 8 Tad = 1 us
    ADCON3Lbits.REFSEL = 0; // AVdd / AVss reference (0 to 3.3 V)
    ADCON1Hbits.FORM = 0; // integer output, 0 to 4095
    ADTRIG2Hbits.TRGSRC10 = 1; // AN10 is converted on the common software trigger (SWCTRG)
    ADCON1Lbits.ADON = 1;
    ADCON5Hbits.WARMTIME = 8;
    ADCON5Lbits.SHRPWR = 1; // power up the shared core
    while (!ADCON5Lbits.SHRRDY);
    ADCON3Hbits.SHREN = 1;

    // --- SCCP2 as the sample timer ---
    // 16-bit timer clocked by Fp, the period flag CCT2IF is set every
    // FCY / sample_rate cycles (2000 cycles = 125 us at 8 kHz)
    CCP2CON1L = 0;
    CCP2CON1H = 0;
    CCP2CON2L = 0;
    CCP2CON2H = 0;
    CCP2CON1Lbits.CCSEL = 0; // timer / output compare mode
    CCP2CON1Lbits.MOD = 0; // 16-bit timer
    CCP2CON1Lbits.T32 = 0;
    CCP2CON1Lbits.CLKSEL = 0; // Fp
    CCP2CON1Lbits.TMRPS = 0; // 1:1
    CCP2TMRL = 0;
    CCP2PRL = (uint16_t) ((FCY / sample_rate) - 1);
    IFS1bits.CCT2IF = 0;
    CCP2CON1Lbits.CCPON = 1;
}

uint16_t mic_read(void) {
    ADCON3Lbits.SWCTRG = 1; // sample, then convert AN10
    while (!ADSTATLbits.AN10RDY);
    return ADCBUF10; // reading the buffer clears AN10RDY
}

void mic_capture(uint16_t *buf, uint16_t len) {
    uint16_t i;

    // the T1 interrupt (1 ms tick) would delay some samples and add jitter,
    // hold it during the capture (msTicks does not count during this time)
    uint8_t t1ie = IEC0bits.T1IE;
    IEC0bits.T1IE = 0;

    CCP2TMRL = 0;
    IFS1bits.CCT2IF = 0;

    for (i = 0; i < len; i++) {
        while (!IFS1bits.CCT2IF); // wait for the next sample time
        IFS1bits.CCT2IF = 0;
        buf[i] = mic_read();
    }

    IEC0bits.T1IE = t1ie;
}

static void send_u16(uint16_t v) {
    UartSendByte((char) (v & 0xFF));
    UartSendByte((char) (v >> 8));
}

void mic_send_frame(const uint16_t *buf, uint16_t len, uint16_t seq) {
    uint16_t i, checksum = 0;

    UartSendByte((char) MIC_SYNC1);
    UartSendByte((char) MIC_SYNC2);
    send_u16(seq);
    send_u16(len);
    send_u16(micSampleRate);

    for (i = 0; i < len; i++) {
        send_u16(buf[i]);
        checksum += buf[i];
    }

    send_u16(checksum);
}
