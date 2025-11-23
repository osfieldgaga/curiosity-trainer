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
#include <stdint.h>

int main(void) {

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

        print_seven_seg(c, c, c, c);

    }
    return 0;
}
