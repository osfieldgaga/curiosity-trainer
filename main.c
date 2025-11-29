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
#include "pushbuttons.h"
#include "led.h"

#include "examples.h"

int main(void) {

    //    example1(); // keypad + 7 segment display
    //    example2(); // 7 segment display counter
    //    example3(); // photoresistor + seven segment
    //    example4(); // potentiometer + seven segment
    //    example5(); // buzzer single note
    //    example6(); // buzzer with music
    //    example7(); // center button + counter on 7 seg
//    example8(); // all buttons + counter on 7 seg
    
    LED_Init();
    
    int state = LED_ON;
    
    while(1) {
     
        
        set_all_leds_state(LED_ON);  
        __delay_ms(500);
        set_all_leds_state(LED_OFF);  
        __delay_ms(500);
    }


    return 0;
}
