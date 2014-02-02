/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 01-24-2014
 *           By: Ching Yuen Ng/ Brian Rak
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
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_button.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <inttypes.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}



int main(void) { 
  //driver init
  f3d_uart_init();
  f3d_led_init();
  f3d_button_init();
  f3d_gyro_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  float buffer[2];

  while (1) {
    f3d_gyro_getdata(buffer);
    delay();
    printf("x: %f y: %f z: %f\n", buffer[0], buffer[1], buffer[2]);
  }

/*
  char* di[]={"N\n","NE\n","E\n","SE\n","S\n","SW\n","W\n","NW\n"};
  int led=0;
  int buffer = '\0';

  
  while(1){
    buffer = readchar();

    //while the button not being pressed
    if(!f3d_button_read()){
      f3d_led_on(led);
      delay();
      f3d_led_off(led);
      printf(di[led++]);
    }

    //if p is pressed
    if (buffer == 'p'){
      printf("p\n");
      f3d_led_on(led);
      printf(di[led]);
      while (buffer != 'r') {
	buffer = readchar();
	delay();
      }
      printf("r\n");
    }
    
    //go back to the north led if it has finished a round
    if(led>7){
      f3d_led_all_on();
      delay();
      f3d_led_all_off();
      delay();
      delay();
      led=0;
    }

    //if the button is being pressed and p is not pressed, 
    //the current led will stay on
    else{
      f3d_led_on(led);
    }
   
  }*/
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
