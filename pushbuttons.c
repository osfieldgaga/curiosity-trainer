/*
 * File:   pushbuttons.c
 * Author: Osfield
 *
 * Created on November 28, 2025, 6:40 PM
 */


#include "xc.h"
#include "config.h"
#include "pushbuttons.h"

void BTN_Center_Init() {
    BTN_C_TRIS = TRIS_INPUT;
}

uint8_t read_center_btn() {
    return BTN_C;
}

uint8_t read_up_btn() {
    return BTN_U;
}

uint8_t read_down_btn() {
    return BTN_D;
}

uint8_t read_left_btn() {
    return BTN_L;
}

uint8_t read_right_btn() {
    return BTN_R;
}
