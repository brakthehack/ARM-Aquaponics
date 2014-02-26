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
int reset_count=0;
*/

void f3d_systick_init(void) {
  // setup systick rate of 100hz.
  SysTick_Config(SystemCoreClock/SYSTICK_INT_SEC);
}

void SysTick_Handler(void) {

    static state = 0;
    state ^= 1;

    /*
    if (state) {
        //f3d_buzzer_on();
        f3d_led_all_on();
    }
    else {
        //f3d_buzzer_off();
        f3d_led_all_off();
    }

    */

    /*
  if(startLog){
    if (state)  {
      //GPIOE->BSRR = 0x8000;
      f3d_led_on(0);
    } else {
      //GPIOE->BRR = 0x8000;
      f3d_led_off(0);
    }
  }

  if(!startLog){
    f3d_led_all_off();
  }

  systick_flag = 1;

  get_data(1); // store gyro data
  if (startLog) {
    log_data(1); // log gyro data
  }
  // since we are only collecting 1/10 as many samples, we can use less space
  if (count++ % 10 == 0) {
    get_data(2); // get altitude data
    if (startLog) {
      log_data(2); // log altitude data
    }
  }

  if(f3d_button_read()){
    reset_count++;
  }

  if(!f3d_button_read()){
    reset_count=0;
  }
    */

  if (!queue_empty(&txbuf)) {
    flush_uart();
  }


  
}

/* f3d_systick.c ends here */
