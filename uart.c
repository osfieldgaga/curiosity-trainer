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
    U1MODE = 0;
    U1MODEH = 0;    
    U1STA = 0;
    U1STAH = 0;
    U1BRG = 25;  // 9600 @ Fosc 200MHz

    U1STAHbits.URXISEL = 6;
    U1MODEHbits.BCLKSEL = 0;
    U1MODEbits.UTXEN = 1;
    U1MODEbits.URXEN = 1;
    U1MODEbits.UARTEN = 1;
    
    //DEBUGGER USB RX/TX TEST - FOR OOB DEMO EFFORT
    RPOR13bits.RP58R = 1;
    RPINR18bits.U1RXR = 59;
}

void UartSendByte(char data)
{
    while (U1STAbits.TRMT == 0)
    {
    } // wait for room in TX buffer

    U1TXREG = data;
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
