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
#include <stdio.h>
#include <math.h>

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
#include "io_expander.h"
#include "oled.h"
#include "accelerometer.h"
#include "mic.h"
#include "ultrasonic.h"
#include "neopixels.h"
#include "uart.h"


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




// The exit condition for the while loops is now handled by the T1 ISR (config.c):
// a center pushbutton press clears "running", even if the example is busy in a
// delay. check_for_exit() is kept so the existing examples still compile, it
// no longer needs to do anything

void check_for_exit() {
}

// Wait for ms milliseconds, or less if the center button ends the example.
// Use it instead of __delay_ms() for long delays in the examples, so they exit
// right away. Returns 0 if the example should stop, 1 otherwise

uint8_t delay_ms_or_exit(uint16_t ms) {
    uint16_t start = msTicks;

    // the cast keeps the subtraction correct when msTicks wraps around
    while (running && (uint16_t) (msTicks - start) < ms);

    return running;
}

// Example 1: Read keypad input and display it on the 7 segment display

void example0() {
    uint16_t refreshCounter = 0;
    char onlineBanner[32];
    uint8_t uptime_s = 0, uptime_m = 0, uptime_h = 0, uptime = 0;
    running = 1;
    while (running) {

        check_for_exit();

        if (tFlag) { //tFlag set to 1 second by default
            uptime++;
            tFlag = 0;
        }

        uptime_s = uptime % 60;
        uptime_m = uptime / 60;
        uptime_h = uptime_m / 60;

        oled_draw_bitmap(bulldog2);
        sprintf(onlineBanner, "Up: %dh %dm %ds", uptime_h, uptime_m, uptime_s);
        oled_draw_string(7, onlineBanner);
        // refresh the screen at most every 16 ms. oled_update() itself takes
        // ~79 ms (measured), so the screen is refreshed ~12 times per second
        if ((uint16_t) (msTicks - refreshCounter) > 16) {

            refreshCounter = msTicks;
            oled_update();
        }

    }
}

void example1() {

    init_seven_seg();
    init_keypad();

    unsigned char c = 0, key_in;
    uint8_t warned = 0;
    running = 1;
    while (running) {

        // the slide switches share the row pins, if any of them is on the
        // keys can't be read reliably, so warn the user and ignore the keypad
        if (keypad_check_rows()) {
            if (!warned) {
                oled_draw_string(4, "Keypad blocked!");
                oled_draw_string(5, "Set SW0-SW3 low");
                oled_update();
                warned = 1;
            }
            key_in = 0;

            // flash "----" at 1 Hz, msCount goes from 0 to 999 every second
            if (msCount < 500) {
                print_seven_seg(SEG_DASH, SEG_DASH, SEG_DASH, SEG_DASH);
            } else {
                print_seven_seg(SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK);
            }
        } else {
            if (warned) {
                oled_clear_line(4);
                oled_clear_line(5);
                oled_update();
                warned = 0;
            }
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
    uint8_t on = 0;
    uint16_t on_cnt = 0;

    turn_off_rgb();
    while (running) {
        check_for_exit();

        if ((uint16_t) (msTicks - on_cnt) > 500) {
            on = ~on;
            on_cnt = msTicks;
        }

        if (on) {
            set_all_leds(0b10101010);
        } else {
            set_all_leds(0b01010101);
        }


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
        check_for_exit();

        set_rgb(SW1, SW2, SW3);
        __delay_ms(10); // small delay just to avoid pegging CPU / for debounce
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

        delay_ms_or_exit(2000);

        check_for_exit();
    }
}

// Example 13: Display the distance measured by the ultrasonic sensor on the 7 segment
// display and on UART

void example13() {

    init_ultrasonic();
    init_seven_seg();


    char buffer[20] = {0};
    UartInit();
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

// Example 14: IO expander Read + Display on OLED

void example14() {

    init_oled();

    // all 8 pins are inputs after init_IO_expander()
    if (!init_IO_expander()) {
        oled_draw_string(6, "TCA9534 not found");
        oled_update();
    }

    uint8_t pins;
    uint16_t refreshCounter = 0;

    while (running) {

        pins = io_read();


        for (int i = 0; i < 8; i++) {
            char buffer = 0;
            buffer = (pins & (1 << i)) ? '1' : '0';
            oled_draw_string(1, "IO Expander inputs:");
            oled_draw_char((7 - i) * 6, 2, buffer);

        }

        // refresh the screen at most every 16 ms. oled_update() itself takes
        // ~79 ms (measured), so the screen is refreshed ~12 times per second
        if ((uint16_t) (msTicks - refreshCounter) > 16) {

            refreshCounter = msTicks;
            oled_update();
        }
        check_for_exit();
    }


}


// Example 15: Control the neopixel neopixels

void example15() {
    //    init_seven_seg();

    NEO_Init();

    sendRGBMultiple(0, 0, 0, 255, 4);
    __delay_ms(100);
    delay_ms_or_exit(1000);

    while (running) {
        //Step 1: Perform the pattern (turn on the first 6 LEDs)
        performPattern(255, 206, 2, 0, 4);
        // Delay to reset
        __delay32(640);
        // Step 2: Turn off the first LEDs
        //                    sendRGBMultiple(0, 0, 0, 0, 4);
        delay_ms_or_exit(500);

        performPattern(35, 75, 153, 0, 4);
        // Delay to reset
        __delay32(640);
        // Step 2: Turn off the first LEDs
        //                    sendRGBMultiple(0, 0, 0, 0, 4);
        delay_ms_or_exit(500);

        performPattern(81, 202, 245, 0, 4);
        // Delay to reset
        __delay32(640);
        // Step 2: Turn off the first LEDs
        //                    sendRGBMultiple(0, 0, 0, 0, 4);
        delay_ms_or_exit(500);
        
        check_for_exit();

    }
    
    // turn off before moving on
    sendRGBMultiple(0, 0, 0, 0, 4); 
}


// Example 16: Accelerometer (MMA8451) Read + Display on OLED

#define RAD_TO_DEG 57.29578f

void example16() {

    accel_data_t accel;
    char buffer[22]; // 21 characters per line on the OLED
    uint16_t refreshCounter = 0;
    float pitch, roll;

    init_oled();

    if (!init_accelerometer()) {
        oled_draw_string(6, "MMA8451 not found");
        oled_update();
    }

    while (running) {

        accel_read(&accel);

        // tilt angles from the direction of gravity, in degrees
        // pitch: rotation around Y, roll: rotation around X
        pitch = atan2f(-accel.x, sqrtf((float) accel.y * accel.y + (float) accel.z * accel.z)) * RAD_TO_DEG;
        roll = atan2f(accel.y, accel.z) * RAD_TO_DEG;

        // fixed width so shorter values overwrite longer ones
        sprintf(buffer, "X:%5d Y:%5d mg", accel_to_mg(accel.x), accel_to_mg(accel.y));
        oled_draw_string(1, buffer);
        sprintf(buffer, "Z:%5d mg", accel_to_mg(accel.z));
        oled_draw_string(2, buffer);
        sprintf(buffer, "Pitch: %4d deg", (int) pitch);
        oled_draw_string(4, buffer);
        sprintf(buffer, "Roll:  %4d deg", (int) roll);
        oled_draw_string(5, buffer);

        // refresh the screen at most every 16 ms. oled_update() itself takes
        // ~79 ms (measured), so the screen is refreshed ~12 times per second
        if ((uint16_t) (msTicks - refreshCounter) > 16) {

            refreshCounter = msTicks;
            oled_update();
        }
        check_for_exit();
    }
}

// Example 17: Capture microphone frames and stream them over UART
// Run tools/mic_viewer.py (Python) or tools/mic_viewer.m (MATLAB) on the
// computer to plot the waveform and its spectrum. The selector switch must be
// set to the microphone (RB8 is shared with the ultrasonic echo)

void example17() {

    // static: 1 KB is large for the stack, far: keeps the 4 KB of near data
    // memory for the other drivers' variables
    static uint16_t __attribute__((far)) frame[MIC_FRAME_LEN];
    uint16_t seq = 0, i, min, max;
    char buffer[22]; // 21 characters per line on the OLED

    mic_init(MIC_SAMPLE_RATE);
    UartInitBaud(MIC_UART_BAUD);

    sprintf(buffer, "%u Hz, %u samples", MIC_SAMPLE_RATE, MIC_FRAME_LEN);
    oled_draw_string(1, buffer);
    sprintf(buffer, "UART %lu baud", (unsigned long) MIC_UART_BAUD);
    oled_draw_string(2, buffer);
    oled_draw_string(3, "Run mic_viewer");
    oled_update();

    running = 1;
    while (running) {

        mic_capture(frame, MIC_FRAME_LEN);
        mic_send_frame(frame, MIC_FRAME_LEN, seq);

        // show the peak to peak level about once per second
        if ((seq & 0x07) == 0) {
            min = 4095;
            max = 0;
            for (i = 0; i < MIC_FRAME_LEN; i++) {
                if (frame[i] < min) min = frame[i];
                if (frame[i] > max) max = frame[i];
            }
            sprintf(buffer, "Level: %4u mV p-p", (uint16_t) (((uint32_t) (max - min) * 3300) / 4095));
            oled_draw_string(5, buffer);
            sprintf(buffer, "Frames sent: %u", seq);
            oled_draw_string(6, buffer);
            oled_update();
        }

        seq++;
    }

    UartInit(); // back to the default 9600 baud
}
