/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Ching Yuen Ng/ Brain Rak
 * Created: 
 * Last-Updated: 
 *           By: 
 *     Update #
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
#include <f3d_led.h>
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <f3d_uart.h>
#include <f3d_i2c.h>
#include <stdio.h>


//draw circle funtion 
void draw_circle(int radius, int ox, int oy, uint16_t color){
  int x,y;
  for (x=-radius; x<radius; x++){
    int height=(int)sqrt(radius*radius-x*x);
    for(y=-height;y<height;y++){
      f3d_lcd_drawPixel(x+ox,y+oy,color);
    }
  }
}


int main() { 
  f3d_led_init();
  f3d_lcd_init();
  f3d_systick_init();
  f3d_uart_init();
  f3d_i2c1_init();
 

 
  /*
  
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawChar(20,80,'X',RED,WHITE);
  f3d_lcd_drawChar(60,80,'Y',GREEN,WHITE);
  f3d_lcd_drawChar(100,80,'Z',BLUE,WHITE);
  f3d_lcd_drawString(50,10,"GYRO",BLACK,WHITE);

  int k=0;
  */
  while(1){
    f3d_led_all_on();
    printf("a\n");
    //f3d_lcd_fillScreen(BLUE);
    //f3d_lcd_fillScreen(RED);
    //f3d_lcd_fillScreen(GREEN);
    
  }
    
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
