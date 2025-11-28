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

int main(void) {

    init_T1();
    init_seven_seg();
    //    init_keypad();

    LDR_Init();

    //    unsigned char c, key_in;

    int ldr;

//    unsigned char c, key_in;
//
    while (1) {

        //        key_in = readKey(KEYPAD_OUTPUT_CHAR);
        //
        //        if (c != key_in && key_in != 0) {
        //            if (key_in - '0' < 10) {
        //                c = key_in - '0';
        //            } else {
        //                c = key_in - 'A' + 10;
        //            }
        //        }


        ldr = read_ldr(0, 1023);


        // 1. Calculate the digits
        int thousands = ldr / 1000; // e.g., 2567 / 1000 = 2
        int hundreds = (ldr % 1000) / 100; // e.g., 567 / 100 = 5
        int tens = (ldr % 100) / 10; // e.g., 67 / 10 = 6
        int ones = ldr % 10; // e.g., 7 / 1 = 7

        // 2. Pass the decimal digits to your display function
        // Assuming your print_seven_seg takes four decimal digits (0-9)
        print_seven_seg(thousands, hundreds, tens, ones);
        //        print_seven_seg(3, 3, 3, 3);



    }
    return 0;
}
