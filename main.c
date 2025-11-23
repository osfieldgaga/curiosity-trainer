/*
 * File:   main.c
 * Author: Osfield
 *
 * Created on November 22, 2025, 4:02 PM
 */


#include "xc.h"
#include "config.h"
#include "seven_segment.h"

int main(void) {

    init_T1();
    init_seven_seg();


    while (1) {

        print_seven_seg(2, 5, 7, 8);

    }
    return 0;
}
