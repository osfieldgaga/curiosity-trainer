/*
 * File:   utils.c
 * Author: Osfield
 *
 * Created on November 22, 2025, 4:02 PM
 */


#include "xc.h"
#include "utils.h"
#include "config.h"
#include <stdint.h>
#include "seven_segment.h"

uint16_t delayCounterMs;
uint8_t enDelay = 0;

void msDelay(int ms) {
    delayCounterMs = ms;
    enDelay = 1;

    while (delayCounterMs > 0);

    enDelay = 0;

}

