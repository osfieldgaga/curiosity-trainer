/* **************************************************************************
 *
 *  @Company
 *    Kettering University
 *
 *  @File Name
 *    potentiometer.h
 *
 *  @Summary
 *    Defines function prototypes for the push buttons
 *
 *  @Description
 *    The functions used for interfacing the push buttons and the function prototypes
 *    are defined in this header file.
 *
 *  @Author
 *    Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 28, 2025, 6:40 PM
 * 
 *  @Last modification
 *      December 09, 2025, 7:21 PM
/* ************************************************************************** */


#include "xc.h"
#include "config.h"
#include "pushbuttons.h"

void BTN_Init() {
    BTN_C_TRIS = TRIS_INPUT;
    BTN_U_TRIS = TRIS_INPUT;
    BTN_D_TRIS = TRIS_INPUT;
    BTN_L_TRIS = TRIS_INPUT;
    BTN_R_TRIS = TRIS_INPUT;
}

void BTN_Center_Init () {
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
