/*
 * File:   examples.c
 * Author: Osfield
 *
 * Created on November 28, 2025, 5:17 PM
 */


#include "xc.h"
#include "examples.h"

#include "config.h"
#include "seven_segment.h"
#include "keypad.h"
#include "pwm.h"
#include <stdint.h>
#include "potentiometer.h"
#include "photoresistor.h"

// Display the digit input by the keyboard on the 7 segment display

void example1() {
    init_T1();
    init_seven_seg();
    init_keypad();

    unsigned char c, key_in;

    while (1) {

        key_in = readKey(KEYPAD_OUTPUT_CHAR);

        if (c != key_in && key_in != 0) {
            if (key_in - '0' < 10) {
                c = key_in - '0';
            } else {
                c = key_in - 'A' + 10;
            }
        }

        print_seven_seg(0, 0, 0, c);

    }
}

void example2() {
    init_T1();
    init_seven_seg();

    int counter = 0;

    while (1) {



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
    }
}

void example3() {
    init_seven_seg();

    LDR_Init();

    int ldr;
    //
    while (1) {

        ldr = read_ldr(0, 1023);


        // Calculate the digits
        int thousands = ldr / 1000;
        int hundreds = (ldr % 1000) / 100;
        int tens = (ldr % 100) / 10;
        int ones = ldr % 10;

        print_seven_seg(thousands, hundreds, tens, ones);

    }
}

void example4() {
    init_seven_seg();

    POT_Init();

    int pot;
    //
    while (1) {

        pot = read_pot(0, 1023);


        // Calculate the digits
        int thousands = pot / 1000;
        int hundreds = (pot % 1000) / 100;
        int tens = (pot % 100) / 10;
        int ones = pot % 10;

        print_seven_seg(thousands, hundreds, tens, ones);

    }
}

// play a single note

void example5() {

    // Initializing sets the PWM frequency to correspond to middle C
    PWM4_Initialize();
    while (1) {

    }
}

// play a sequence of notes

void example6() {

    PWM4_Initialize();

    while (1) {

        play_music();
    }
}

void example7() {
    init_T1();
    init_seven_seg();
    BTN_Center_Init();

    int counter = 0;

    while (1) {

        if (read_center_btn() == 0) {
            while (read_center_btn() == 0);

            counter++;
        }

        // Calculate the digits
        int thousands = counter / 1000;
        int hundreds = (counter % 1000) / 100;
        int tens = (counter % 100) / 10;
        int ones = counter % 10;

        print_seven_seg(thousands, hundreds, tens, ones);
    }
}

void example8() {
    init_T1();
    init_seven_seg();
    BTN_Center_Init();

    int counter = 0;

    while (1) {

        if (read_center_btn() == 0 || read_up_btn() == 0 || read_down_btn() == 0 || read_left_btn() == 0 || read_right_btn() == 0) {
            while (read_center_btn() == 0 || read_up_btn() == 0 || read_down_btn() == 0 || read_left_btn() == 0 || read_right_btn() == 0);

            counter++;
        }

        // Calculate the digits
        int thousands = counter / 1000;
        int hundreds = (counter % 1000) / 100;
        int tens = (counter % 100) / 10;
        int ones = counter % 10;

        print_seven_seg(thousands, hundreds, tens, ones);
    }
}
