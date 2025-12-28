/*
 * 
 *  @Company
        Kettering University
 * 
 *  @File Name
 *      keypad.c
 * 
 *  @Summary
 *      This file sets up the keypad and provides a function to read the input keys 
 * 
 *  @Description
 *      The keypad.c file contains the main initialization and configuration of 
 *      the keypad. It provides a function to configure the IO pins and read the
 *      input keys. By default, the keys are not debounced.
 *  @Authors: 
 *      Osfield Gaga & Jeremy Gooch
 *
 *  @Created on
 *      November 23, 2025, 2:03 PM
 * 
 *  @Last modification
 *      December 09, 2025, 11:35 AM
 */


#include "xc.h"
#include "keypad.h"
#include "config.h"
#include <libpic30.h>

// The layout of the keypad. Can be adjusted to return any character as required
unsigned char keys[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'A', '0', 'B'}
};

void init_keypad() {
    COL1_TRIS = TRIS_OUTPUT;
    COL2_TRIS = TRIS_OUTPUT;
    COL3_TRIS = TRIS_OUTPUT;
    //        COL4_TRIS = TRIS_OUTPUT;

    ROW1_TRIS = TRIS_INPUT;
    ROW2_TRIS = TRIS_INPUT;
    ROW3_TRIS = TRIS_INPUT;
    ROW4_TRIS = TRIS_INPUT;

    COL1_ANSEL = ANSEL_DIGITAL;
    COL2_ANSEL = ANSEL_DIGITAL;
    COL3_ANSEL = ANSEL_DIGITAL;
    //    COL4_ANSEL = ANSEL_DIGITAL;

    //        ROW1_ANSEL = ANSEL_DIGITAL;
    //        ROW2_ANSEL = ANSEL_DIGITAL;
    ROW3_ANSEL = ANSEL_DIGITAL;
    ROW4_ANSEL = ANSEL_DIGITAL;

}

/*******************************************************************************
 * Read keypad input
 * 
 * Iterate through the columns and assess which button on which row is down
 * The function can either output the raw character that was pressed, or the
 * index equivalent decimal value of the character 
 * 
 * Parameters:
 * outputMode: 0 outputs the raw character and 1 outputs the decimal value of 
 * the character
 * 
 * Using the decimal value is used to select the appropriate character to display
 * on the 7 segment display
 ******************************************************************************/
unsigned char readKey(uint8_t outputMode) {

    unsigned char keyout = 0;

    //loop indexes for the for loops required for scanning columns and rows
    int i, j;

    //outer for loop for the columns
    for (i = 0; i < 3; i++) {

        if (keyout != 0) {
            break;
        }
        //init coll values to one
        COL1 = COL_OFF;
        COL2 = COL_OFF;
        COL3 = COL_OFF;
        //        COL4 = COL_OFF;
        //turn on the associated columns (1 per iteration)
        switch (i) {
            case 0:
            {
                COL1 = COL_ON;
                COL2 = COL_OFF;
                COL3 = COL_OFF;
                break;
            }
            case 1:
            {
                COL1 = COL_OFF;
                COL2 = COL_ON;
                COL3 = COL_OFF;
                break;
            }
            case 2:
            {
                COL1 = COL_OFF;
                COL2 = COL_OFF;
                COL3 = COL_ON;
                break;
            }
                //            case 3: COL4 = COL_ON;
                //                break;
            default:;
                break;
        }
        //for loop to iterate over the rows in each column
        for (j = 0; j < 4; j++) {
            //if we get a row value that is low, we record the keyout value
            //and break out of the loop

            switch (j) {
                case 0:
                    if (ROW1 != ROW_OFF) {
                        keyout = keys[j][i];
                    }
                    break;
                case 1:
                    if (ROW2 != ROW_OFF) {
                        keyout = keys[j][i];

                    }
                    break;
                case 2:
                    if (ROW3 != ROW_OFF) {
                        keyout = keys[j][i];

                    }
                    break;
                case 3:
                    if (ROW4 != ROW_OFF) {
                        keyout = keys[j][i];

                    }
                    break;

                default:

                    break;
            }
        }


    }

    // check what to output

    if (keyout == 0) {
    } else {
        if (outputMode == KEYPAD_OUTPUT_DECIMAL) {
            // return the decimal value that corresponds to
            // the character entered
            if (keyout - '0' < 10) {
                keyout = keyout - '0';
            } else {
                keyout = keyout - 'A' + 10;
            }
        } else if (outputMode == KEYPAD_OUTPUT_CHAR) {
            // this output mode just return the raw char entered
            // since keyout is already set to the raw char, no need to do anything 
            // else, but here you can add extra processing if required
        }
    }

    return keyout;
}
