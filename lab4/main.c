/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 02-2-2014
 *           By: Ching Yuen Ng/ Brian Rak
 *     Update #: 2
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
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_button.h>
#include <f3d_gyro.h>
#include <f3d_systick.h>
#include <f3d_pressure.h>

#include <stdio.h>


// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif


int main(void) { 
  f3d_uart_init();
  f3d_led_init();
  f3d_button_init();
  f3d_gyro_init();
  f3d_systick_init();
  f3d_pressure_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  float temp, pressure;
  uint8_t buffer;

  while (1) {

    f3d_pressure_getdata(&pressure, &temp);
		f3d_pressure_read(&buffer, 0x27, 1);
		printf("%x\n", buffer);
		//printf("Temp (C): %f Pressure: %f\n",temp,pressure);
 
  }
}
/* main.c ends here */
