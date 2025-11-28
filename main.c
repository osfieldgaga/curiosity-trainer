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

#define POT PORTAbits.RA3
#define POT_TRIS TRISAbits.TRISA3
#define POT_ANSEL ANSELAbits.ANSELA3

int main(void) {

    init_T1();
    init_seven_seg();
    init_keypad();

    POT_TRIS = TRIS_INPUT;
    POT_ANSEL = ANSEL_ANALOG;

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

        //        print_seven_seg((POT & 0xF000) >> 12, (POT & 0xF00) >> 8, (POT & 0xF0) >> 4, POT & 0xF);

        if (POT > 0) {
            print_seven_seg(1, 1, 1, 1);
        } else {
            print_seven_seg(5, 5, 2, 2);
        }

    }
    return 0;
}
