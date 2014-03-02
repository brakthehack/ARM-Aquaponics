/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Sep 26 07:05:26 2013
 * Last-Updated: Ching Yuen Ng/ Brian Rak
 *           By: 02-7-2014
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */


/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include <f3d_systick.h>
#include <f3d_led.h>
#include <f3d_buzzer.h>
#include <f3d_uart.h>
#define SYSTICK_INT_SEC 100 // .1 second pulse 10Hz

// buffers here
/*
volatile int systick_flag = 0;
volatile uint8_t count=0;
uint8_t startLog = 0;
*/
int hold_count=0;

/*
 * User button State
 */
extern uint8_t button_check_state, button_check_state2;
extern volatile uint16_t button_state, button_state2;
extern volatile uint16_t time_passed, time_passed2;

void f3d_systick_init(void) {
    // setup systick rate of 100hz.
    SysTick_Config(SystemCoreClock/SYSTICK_INT_SEC);
}

void SysTick_Handler(void) {
    // changes the state machine for user button presses
    if (button_check_state) {
        uint8_t cur_state = f3d_button_read();
        button_state = cur_state ? button_state + cur_state : 0;
        time_passed++;
    }
    if (button_check_state2) {
        uint8_t cur_state2 = f3d_extra_button();
        button_state2 = cur_state2 ? button_state2 + cur_state2 : 0;
        time_passed2++;
    }

    // end button press state machine

    if(f3d_button_read()){
        hold_count++;
    }

    if(!f3d_button_read()){
        hold_count=0;
    }


    if (!queue_empty(&txbuf)) {
        flush_uart();
    }
}

/* f3d_systick.c ends here */
