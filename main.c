/*
 * File:   main.c
 * Author: Osfield
 *
 * Created on November 22, 2025, 4:02 PM
 */


#include "xc.h"
#include "config.h"
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

    init_seven_seg(); // to display state and config
    BTN_Center_Init(); // to change state
    UartInit();

    uint8_t state = 0;

    while (1) {

        switch (state) {
            case 0:
            {
                seven_seg_enable = 1;
                print_decimal_point(DP_OFF, DP_ON, DP_OFF, DP_OFF);

                example13();
                break;
            }
            case 1:
            {

                print_decimal_point(DP_OFF, DP_ON, DP_OFF, DP_OFF);

                example1(); // keypad + 7 segment display
                break;
            }
            case 2:
            {

                print_decimal_point(DP_OFF, DP_ON, DP_OFF, DP_OFF);
                example2(); // 7 segment display counter

                break;
            }
            case 3:
            {
                print_decimal_point(DP_OFF, DP_ON, DP_OFF, DP_OFF);

                example3(); // photoresistor + seven segment
                break;
            }
            case 4:
            {
                print_decimal_point(DP_OFF, DP_ON, DP_OFF, DP_ON);

                example4(); // potentiometer + seven segment
                break;
            }
            case 5:
            {
                print_decimal_point(DP_ON, DP_ON, DP_OFF, DP_OFF);

                example5(); // buzzer single note
                //                example14();
                break;
            }
            case 6:
            {
                print_decimal_point(DP_OFF, DP_ON, DP_OFF, DP_OFF);

                example8(); // UDLR buttons + counter
                break;
            }
            case 7:
            {
                print_decimal_point(DP_OFF, DP_OFF, DP_OFF, DP_OFF);

                seven_seg_enable = 0;
                example9(); // all LEDs blinker
                break;
            }
            case 8:
            {
                print_decimal_point(DP_OFF, DP_OFF, DP_ON, DP_OFF);

                seven_seg_enable = 0;
                example10(); // switch + LEDs
                break;
            }
            case 9:
            {
                print_decimal_point(DP_OFF, DP_OFF, DP_ON, DP_OFF);

                seven_seg_enable = 0;
                example11(); // switch + RGB
                break;
            }

        }


        // If we are here, it means check_for_exit() was triggered.
        // So we increment the state immediately before looping back!
        state++;
        if (state > 9) state = 0;
        PWM4_Disable();

        // Reset running for the next state
        running = 1;

    }

    //    example1(); // keypad + 7 segment display
    //    example2(); // 7 segment display counter
    //    example3(); // photoresistor + seven segment
    //    example4(); // potentiometer + seven segment
    //    example5(); // buzzer single note
    //    example6(); // buzzer with music
    //    example7(); // center button + counter on 7 seg
    //    example8(); // all buttons + counter on 7 seg
    //    example9(); // all LEDs blinker
    //    example11(); //rgb 
    //    example13(); //UART 

    return 0;
}
