/*
 * File:   main.c
 * Author: Osfield
 *
 * Created on November 22, 2025, 4:02 PM
 */


#include "xc.h"
#include "config.h"
#include "oled.h"
#include "seven_segment.h"
#include "keypad.h"
#include "pwm.h"
#include <stdint.h>
#include "potentiometer.h"
#include "photoresistor.h"
#include "pushbuttons.h"
#include "led.h"
#include "switches.h"
#include "ultrasonic.h"
#include "uart.h"
#include <string.h>

#include <stdio.h>

#include "examples.h"

#include "neopixels.h"
#include "accelerometer.h"

int main(void) {

    uint8_t state = 0; // start on the default screen (bulldog + uptime)
    
    init_oled();

    // Configure PLL: FRC (8 MHz) -> Fvco = 8 MHz * M / N1 = 512 MHz,
    // Fpllo = Fvco / (N2 * N3) = 64 MHz. On the dsPIC33CK, Fosc = Fpllo / 2 =
    // 32 MHz and Fcy = Fp = Fosc / 2 = 16 MHz (FCY in config.h)

    CLKDIVbits.PLLPRE = 1;          // N1 = 1
    PLLFBDbits.PLLFBDIV = 64;       // M = 64
    PLLDIVbits.POST1DIV = 4;        // N2 = 4
    PLLDIVbits.POST2DIV = 2;        // N3 = 2

    // Request clock switch to FRC with PLL
    __builtin_write_OSCCONH(0x01);  // NOSC = FRCPLL
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while(OSCCONbits.OSWEN);        // Wait for switch
    while(!OSCCONbits.LOCK);        // Wait for PLL lock

    // Timer 1 runs from now on: it provides the 1 ms tick for the delays, and
    // its ISR watches the center button to end the running example
    init_T1();
    BTN_Center_Init();
    
    while (1) {

        switch (state) {
            case 0:
            {
                // Default screen, bulldog + uptimer
                example0();
                break;
            }

            case 1:
            {
                seven_seg_enable = 1;
                oled_draw_string(0, "Ultrasonic");
                oled_draw_string(1, "Distance on 7 seg");
                oled_draw_string(2, "display and UART");
                oled_update();

                example13();
                break;
            }
            case 2:
            {
                oled_draw_string(0, "Keypad + 7 seg");
                oled_draw_string(1, "Press keys on the");
                oled_draw_string(2, "keypad");
                oled_update();

                example1(); // keypad + 7 segment display
                break;
            }
            case 3:
            {
                oled_draw_string(0, "7 segment counter");
                oled_draw_string(1, "Just count up");
                oled_update();

                example2(); // 7 segment display counter


                break;
            }
            case 4:
            {
                oled_draw_string(0, "Photoresistor");
                oled_update();

                example3(); // photoresistor + seven segment
                break;
            }
            case 5:
            {
                oled_draw_string(0, "Potentiometer");
                oled_update();

                example4(); // potentiometer + seven segment
                break;
            }
            case 6:
            {
                oled_draw_string(0, "Buzzer");
                oled_update();

                example5(); // buzzer single note
                //                example14();
                break;
            }
            case 7:
            {
                oled_draw_string(0, "UDLR btns + counter");
                oled_update();

                example8(); // UDLR buttons + counter
                break;
            }
            case 8:
            {
                oled_draw_string(0, "LEDs blinker");
                oled_update();

                seven_seg_enable = 0;
                example9(); // all LEDs blinker
                break;
            }
            case 9:
            {
                oled_draw_string(0, "Switches + LEDs");
                oled_update();

                seven_seg_enable = 0;
                example10(); // switch + LEDs
                break;
            }
            case 10:
            {
                oled_draw_string(0, "Switches + RGB");
                oled_update();

                seven_seg_enable = 0;
                example11(); // switch + RGB
                break;
            }

            case 11:
            {
                oled_draw_string(0, "IO expander Read");
                oled_update();

                example14(); // Io exp
                break;
            }
            case 12:
            {
                oled_draw_string(0, "Neopixels");
                oled_update();

                example15(); // Neo pixels
                break;              
            }
            case 13:
            {
                oled_draw_string(0, "Accelerometer");
                oled_update();

                example16(); // MMA8451 accelerometer
                break;
            }
            case 14:
            {
                oled_draw_string(0, "UART Hello World");
                oled_draw_string(1, "9600 8N1 on the");
                oled_draw_string(2, "USB virtual COM");
                oled_update();

                example12(); // Hello World on UART
                break;
            }
            case 15:
            {
                oled_draw_string(0, "Microphone");
                oled_update();

                example17(); // mic frames over UART
                break;
            }

        }

        // If we are here, it means the center button was pressed (T1 ISR).
        // So we increment the state immediately before looping back!
        state++;
        if (state > 15) state = 0;
        PWM4_Disable();

        // clear all instructions
        oled_clear(); // clear first
        oled_draw_string(7, "-Center btn to cycle-"); // page 7 = bottom
        oled_update();

        // Reset running for the next state
        running = 1;
    }


    return 0;
}
