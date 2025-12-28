/*
 * 
 *  @Company
        Kettering University
 * 
 *  @File Name
 *      examples.c
 * 
 *  @Summary
 *      Examples to demonstrate the capabilities of the Curiosity Training board
 * 
 *  @Description
 *      Defines all the examples used to demonstrate the capabilities of the
 *      curiosity board, but also provides example implementation for those
 *      components. The example code can be used as a starting point.
 * 
 *  @Authors: 
 *      Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 28, 2025, 5:17 PM
 * 
 *  @Last modification
 *      December 28, 2025, 12:54 PM
 */

#include "examples.h"

#include <stdint.h>

#include "xc.h"
#include "config.h"

#include "seven_segment.h"
#include "keypad.h"

#include "potentiometer.h"
#include "photoresistor.h"
#include "pushbuttons.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"


// Global variable used to enable a specific example. This controls the 
// while loop for each example. Setting it to 0 breaks out of the loop
volatile uint8_t running = 1;


/*
 * The setup for the example follows the following structure
 * 
 * example () {
 * 
 *      setup steps: things that are done only once like initializations
 * 
 *      while (running) {
 *          continuous loop with the main program running continuously
 *      }
 * }
 
 */




// Here is the exit condition for the while loop. By default, it is set to detect
// a Center pushbutton press

void check_for_exit() {
    if (read_center_btn() == 0) {
        while (read_center_btn() == 0); // Wait for release
        running = 0; // This will break the while loops in your examples
    }
}

// Example 1: Read keypad input and display it on the 7 segment display

void example1() {

    init_seven_seg();
    init_keypad();

    unsigned char c = 0, key_in;
    running = 1;
    while (running) {

        key_in = readKey(KEYPAD_OUTPUT_CHAR);

        if (c != key_in && key_in != 0) {
            if (key_in - '0' < 10) {
                c = key_in - '0';
            } else {
                c = key_in - 'A' + 10;
            }
        }

        print_seven_seg(0, 0, 0, c);

        check_for_exit();
    }
}

// Example 2: 7 segment display counter, increments every second

void example2() {
    init_seven_seg();

    int counter = 0;
    running = 1;
    while (running) {

        if (tFlag) {
            tFlag = 0;
            counter++;
        }

        // Calculate the digits
        int thousands = counter / 1000;
        int hundreds = (counter % 1000) / 100;
        int tens = (counter % 100) / 10;
        int ones = counter % 10;

        print_seven_seg(thousands, hundreds, tens, ones);

        check_for_exit();
    }
}

// Example 3: Display the output of the photoresistor on the 7 segment display

void example3() {
    init_seven_seg();

    LDR_Init();

    int ldr;

    while (running) {

        ldr = read_ldr(0, 4096);

        // Calculate the digits
        int thousands = ldr / 1000;
        int hundreds = (ldr % 1000) / 100;
        int tens = (ldr % 100) / 10;
        int ones = ldr % 10;

        print_seven_seg(thousands, hundreds, tens, ones);
        check_for_exit();
    }
}

// Example 4: Display the output of the potentiometer on the 7 segment display

void example4() {
    init_seven_seg();

    POT_Init();

    int pot;
    running = 1;
    while (running) {

        pot = read_pot(0, 100);
        __delay_ms(5);

        // Calculate the digits
        int thousands = pot / 1000;
        int hundreds = (pot % 1000) / 100;
        int tens = (pot % 100) / 10;
        int ones = pot % 10;

        print_seven_seg(thousands, hundreds, tens, ones);

        check_for_exit();

    }
}

// Example 5: Play a single note on the buzzer

void example5() {

    // Initializing sets the PWM frequency to correspond to middle C
    Buzzer_Init();
    while (running) {

        check_for_exit();
    }
}

// Example 6: Play a sequence of notes on the buzzer

void example6() {

    Buzzer_Init();
    running = 1;
    while (running) {

        play_music();
        check_for_exit();
    }
}

// Example 7: A simple button press counter which displays the counter value on 
// the 7 segment display when the up button is pressed

void example7() {
    init_T1();
    init_seven_seg();
    BTN_Init();

    int counter = 0;
    running = 1;
    while (running) {

        if (read_up_btn() == 0) {
            while (read_up_btn() == 0);

            counter++;
        }

        // Calculate the digits
        int thousands = counter / 1000;
        int hundreds = (counter % 1000) / 100;
        int tens = (counter % 100) / 10;
        int ones = counter % 10;

        print_seven_seg(thousands, hundreds, tens, ones);
        check_for_exit();
    }
}

// Example 8: Increment a counter differently based on which button is pressed

void example8() {
    init_T1();
    init_seven_seg();
    BTN_Init();

    int counter = 0;
    running = 1;
    while (running) {

        if (read_up_btn() == 0) {
            while (read_up_btn() == 0);

            if (counter < 9999) {
                counter++;
            }
        }

        if (read_down_btn() == 0) {
            while (read_down_btn() == 0);

            if (counter > 0) {
                counter--;
            }
        }

        if (read_left_btn() == 0) {
            while (read_left_btn() == 0);

            if (counter > 10) {
                counter -= 10;
            } else {
                counter = 0;
            }
        }

        if (read_right_btn() == 0) {
            while (read_right_btn() == 0);

            if (counter < 9989) {
                counter += 10;
            } else {
                counter = 9999;
            }
        }

        // Calculate the digits
        int thousands = counter / 1000;
        int hundreds = (counter % 1000) / 100;
        int tens = (counter % 100) / 10;
        int ones = counter % 10;

        print_seven_seg(thousands, hundreds, tens, ones);
        check_for_exit();
    }
}

// Example 9: LEDs BLinker

void example9() {

    LED_Init();

    running = 1;
    turn_off_rgb();
    while (running) {


        set_all_leds(0b10101010);
        __delay_ms(500);

        set_all_leds(0b01010101);
        __delay_ms(500);

        check_for_exit();
    }
}

// Example 10: Turn on LED corresponding to switch

void example10() {
    LED_Init();
    SW_Init();


    running = 1;
    while (running) {

        int sw = read_switches();
        set_all_leds(sw);
        check_for_exit();
    }
}

// Example 11: Cycle through RGB LED

void example11() {

    LED_Init();
    SW_Init();
    running = 1;
    while (running) {

        set_rgb(1, 0, 0);
        __delay_ms(500);
        set_rgb(0, 1, 0);
        __delay_ms(500);
        set_rgb(0, 0, 1);
        __delay_ms(500);

        check_for_exit();
    }
}

// Example 12: Say Hello on UART

void example12() {

    UartInit();
    __delay_ms(100);
    UartSendString("Welcome to the Curiosity Training Board on UART!");
    UartSendString("\r\n");
    running = 1;
    while (running) {

        UartSendString("Hello World!!");
        UartSendString("\r\n");

        __delay_ms(2000);

        check_for_exit();
    }
}

// Example 13: Display the distance measured by the ultrasonic sensor on the 7 segment
// display and on UART

void example13() {

    init_ultrasonic();
    init_seven_seg();


    char buffer[20] = {0};
    //    UartInit();
    __delay_ms(100);
    UartSendString("\r\n");
    UartSendString("\r\n --------------------- \r\n");
    UartSendString("Welcome!");
    UartSendString("\r\n");
    int d = 0;
    running = 1;
    while (running) {

        d = sense_distance();

        int thousands = d / 1000;
        int hundreds = (d % 1000) / 100;
        int tens = (d % 100) / 10;
        int ones = d % 10;

        print_seven_seg(thousands, hundreds, tens, ones);

        sprintf(buffer, "Distance = %d cm\r\n", d);

        UartSendString(buffer);

        __delay_ms(5);
        check_for_exit();
    }
}


// Example 15: Control the neopixel neopixels

void example15() {
    init_seven_seg();

    NEO_Init();

    while (1) {
        sendPixel(255, 0, 0);
        sendPixel(255, 0, 0);
        sendPixel(255, 0, 0);
        sendPixel(255, 0, 0);

        sendPixel(0, 0, 0);
        show();
    }
}




