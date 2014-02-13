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
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_button.h>
#include <f3d_gyro.h>
#include <f3d_systick.h>
#include <f3d_pressure.h>
//#include <log.h>
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
/*
extern uint8_t gyroIndex;
extern uint8_t altIndex;

void dump_cache(void) {
  int i;
  printf("GYRO\n");
  for (i = 0; i < gyroIndex; i++) {
    printf("%f,%f,%f\n", gyro_cache[i][0], gyro_cache[i][1], gyro_cache[i][2]);
  }
  printf("ALTITUDE\n");
  for (i = 0; i < altIndex; i++) {
    printf("%f\n", altitude_cache[i]);
  }
  printf("Cache dumped\n");
}
*/
int main(void) { 
  f3d_uart_init();
  f3d_led_init();
  f3d_button_init();
  f3d_gyro_init();
  f3d_pressure_init();
  f3d_systick_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  extern uint8_t startLog;
  char c;
  uint8_t button_state = 0;

  while (1) {
    c = readchar();
    if (c == 'x')
      putchar(c);
    //test whoami address, should print out BB
    //f3d_pressure_read(&buffer, 0x0F, 1);
    //printf("%x\n", buffer);

    //f3d_pressure_getdata(&pressure, &temp);
    //printf("Temp (C): %d Pressure: %d\n",temp,pressure);
    /*
    if (c == 'd') {
      printf("Dumping cache...\n");
      dump_cache();
    }
    // press the button to start and stop logging
    if (f3d_button_read()) {
      if (!startLog)
        printf("Log Start\n");
      else
        printf("Log Stop\n");
      startLog ^= 1;
      delay(); // prevent against button being double pressed
    }
     
      // printf("Altitude: %f\n",altitude);
      // printf("x: %f y: %f z: %f\n", gyro_buffer[0], gyro_buffer[1], gyro_buffer[2]);
      // delay();
      // delay();
  */
  }
 
}
/* main.c ends here */
