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

int main(void) {
    init_T1();
    init_seven_seg(); // to display state and config
    BTN_Center_Init(); // to change state
    UartInit();

    uint8_t state = 11;

    init_oled();
    __delay_ms(100);
    oled_fill(0x00); // clear first

    oled_draw_string(7, "-Center btn to cycle-"); // page 7 = bottom
//    oled_draw_bitmap(bulldog2);
    oled_update();

    int refreshCounter = 0;
    char onlineBanner[32];
    uint8_t uptime_s = 0, uptime_m = 0, uptime_h = 0, uptime = 0;

    while (1) {

        switch (state) {
            case 0:
            {
                if (tFlag) { //tFlag set to 1 second by default
                    uptime++;
                    tFlag = 0;
                }

                uptime_s = uptime % 60;
                uptime_m = uptime / 60;
                uptime_h = uptime_m / 60;

                sprintf(onlineBanner, "Up: %dh %dm %ds", uptime_h, uptime_m, uptime_s);
                oled_draw_string(7, onlineBanner);
                // refresh screen at ~60Hz
                if (msCount - refreshCounter > 16) {

                    refreshCounter = msCount;
                    oled_update();
                }
            }
            case 1:
            {
                seven_seg_enable = 1;
                oled_draw_string(0, "Ultrasonic (7seg & UART)");
                oled_update();

                example13();
                break;
            }
            case 2:
            {
                oled_draw_string(0, "Keypad + 7 seg");
                oled_update();

                example1(); // keypad + 7 segment display
                break;
            }
            case 3:
            {
                oled_draw_string(0, "7 seg counter");
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
            
            case 11: {
                oled_draw_string(0, "IO expander Read");
                oled_update();
                
                example14(); // switch + RGB
                break;
            }

        }

        // If we are here, it means check_for_exit() was triggered.
        // So we increment the state immediately before looping back!
        state++;
        if (state > 11) state = 0;
        PWM4_Disable();

        // Reset running for the next state
        running = 1;
    }


    // below was a hacky attempt at making the neopixels work

    //    // 1. Configure the PLL prescaler, postscalers, and feedback divider
    //    
    //    // N1 = 1: Set PLL prescaler to divide by 1 
    //    CLKDIVbits.PLLPRE = 1; 
    //
    //    // M = 60: Set PLL feedback divider to 60 
    //    // Fvco = 8 MHz * 60 = 480 MHz
    //    PLLFBDbits.PLLFBDIV = 60; 
    //
    //    // N2 = 3, N3 = 1: Set PLL postscalers 
    //    // Fosc = 480 MHz / (3 * 1) = 160 MHz
    //    PLLDIVbits.POST1DIV = 3; 
    //    PLLDIVbits.POST2DIV = 1; 
    //
    //    // 2. Initiate Clock Switch to FRC oscillator with PLL (NOSC = 0b001)
    //    __builtin_write_OSCCONH(0x01);
    //    __builtin_write_OSCCONL(OSCCON | 0x01);
    //
    //    // 3. Wait for the clock switch to complete
    //    while (OSCCONbits.COSC != 0b001);
    //
    //    // 4. Wait for the PLL to lock
    //    while (OSCCONbits.LOCK == 0);
    //    
    //    example15();



    return 0;
}
