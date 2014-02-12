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

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
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
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_pressure.h>

#include <pressure_utils.h>

#define SYSTICK_INT_SEC 10
#define LOGGING 1
#define LOG_SIZE 330

#ifdef LOGGING
float altitude_cache[LOG_SIZE];
float gyro_cache[LOG_SIZE][2];

uint8_t ctrl2=0x01;
float pressure, temp, gyro_buffer[2];
float *gyro_ptr = gyro_buffer;
#endif

volatile int systick_flag = 0;
int count=0;

void f3d_systick_init(void) {
  // setup systick rate of 100hz.
  SysTick_Config(SystemCoreClock/SYSTICK_INT_SEC);
}


void SysTick_Handler(void) {
  static state = 0;
  state ^= 1;
  if (state)  {
    //GPIOE->BSRR = 0x8000;
    f3d_led_on(0);
  } else {
    //GPIOE->BRR = 0x8000;
    f3d_led_off(0);
  }
  systick_flag = 1;

  if (LOGGING) {
    static int cacheIndex = 0;
    if (cacheIndex < LOG_SIZE) {
      f3d_pressure_write(&ctrl2, 0x21, 1);
      f3d_pressure_getdata(&pressure, &temp);
      convert_to_altitude_ft(&pressure, &altitude_cache[cacheIndex]);
      //get data from gyro
      f3d_gyro_getdata(&gyro_buffer[0]);
      gyro_cache[cacheIndex][0] = gyro_buffer[0];
      gyro_cache[cacheIndex][1] = gyro_buffer[1];
      gyro_cache[cacheIndex][2] = gyro_buffer[2];
      cacheIndex++;
    }
  }

  if (!queue_empty(&txbuf)) {
    flush_uart();
  }
}

/* f3d_systick.c ends here */
