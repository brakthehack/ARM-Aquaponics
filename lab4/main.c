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

#include <math.h>
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

void Pressure_to_Altitude(float* Pressure_mb, float* Altitude_ft) {
  //=(1-(A18/1013.25)^0.190284)*145366.45
  *Altitude_ft = (1-pow(*Pressure_mb/1013.25,0.190284))*145366.45;
}


int main(void) { 
  f3d_uart_init();
  f3d_led_init();
  f3d_button_init();
  f3d_gyro_init();

  f3d_pressure_init();

  delay();
  f3d_systick_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  extern float temp, pressure;
  uint8_t buffer;
  extern float gyro_buffer[2];

  float altitude;

  while (1) {
    
    //test whoami address, should print out BB
    //f3d_pressure_read(&buffer, 0x0F, 1);
    //printf("%x\n", buffer);

    printf("a\n");

    //f3d_pressure_getdata(&pressure, &temp);
    //printf("Temp (C): %d Pressure: %d\n",temp,pressure);

    Pressure_to_Altitude(&pressure, &altitude);

    printf("%f\n",altitude);
 
    printf("x: %f y: %f z: %f\n", gyro_buffer[0], gyro_buffer[1], gyro_buffer[2]);
  }
}
/* main.c ends here */
