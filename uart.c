/*
 * File:   uart.c
 * Author: Osfield
 *
 * Created on December 10, 2025, 10:35 PM
 */


#include "xc.h"
#include "uart.h"
#include "config.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <libpic30.h>

void UartInit(void)
{
    UartInitBaud(UART_BAUD);
}

void UartInitBaud(uint32_t baud)
{
    U1MODE = 0;
    U1MODEH = 0;
    U1STA = 0;
    U1STAH = 0;

    // Route U1TX/U1RX to the Curiosity Nano debugger's virtual COM port
    // (RC10 = RP58 = TX -> CDC RX, RC11 = RP59 = RX <- CDC TX).
    // PPS must be unlocked first: other drivers (e.g. PWM4_Disable()) lock it,
    // and writes to RPORx/RPINRx are silently ignored while it is locked
    __builtin_write_RPCON(0x0000);  // unlock PPS
    RPOR13bits.RP58R = 1;           // RP58 = U1TX
    RPINR18bits.U1RXR = 59;         // U1RX = RP59
    __builtin_write_RPCON(0x0800);  // lock PPS

    TRISCbits.TRISC10 = TRIS_OUTPUT; // TX
    TRISCbits.TRISC11 = TRIS_INPUT;  // RX
    LATCbits.LATC10 = 1;             // idle high until the UART takes the pin

    // BCLKSEL = 0: the baud rate generator runs from Fp = FCY = 16 MHz
    // BRGH = 1: baud = FCY / (4 * (U1BRG + 1)), rounded to the closest value.
    // The finer divider keeps the error low: 9600 -> 9592 (-0.08%),
    // 115200 -> 114286 (-0.8%), 230400 -> 235294 (+2.1%), 250000 is exact
    U1MODEbits.BRGH = 1;
    U1BRG = (uint16_t)(((FCY + 2UL * baud) / (4UL * baud)) - 1);

    U1STAHbits.URXISEL = 6;
    U1MODEHbits.BCLKSEL = 0;
    U1MODEbits.UTXEN = 1;
    U1MODEbits.URXEN = 1;
    U1MODEbits.UARTEN = 1;
}

void UartSendByte(char data)
{
    while (U1STAHbits.UTXBF)
    {
    } // wait for room in TX buffer

    U1TXREG = data;
}

void UartSendBuffer(const uint8_t *pData, uint16_t len)
{
    uint16_t i;

    for (i = 0; i < len; i++)
    {
        UartSendByte((char) pData[i]);
    }
}

void UartSendString(char *pData)
{
    unsigned int x = 0;

    while(pData[x] != 0)
    {
        UartSendByte(pData[x]);
        x = x + 1;
    }
}

uint8_t UartDataAvailable(void)
{
    // a receive overrun stops the receiver until OERR is cleared, the bytes
    // already in the buffer are kept
    if (U1STAbits.OERR)
    {
        U1STAbits.OERR = 0;
    }

    return !U1STAHbits.URXBE;
}

char UartReadByte(void)
{
    while (!UartDataAvailable())
    {
    } // wait for a byte

    return U1RXREG;
}
