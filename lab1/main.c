/* main.c ---
 *
 * Filename: main.c
 * Description:
 * Author: Ching Yuen Ng/Brian Rak
 * Maintainer:
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated:01/17/2014
 *           By:Ching Yuen Ng
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

#include <stm32f30x.h>
#include <f3d_led.h>
#include <f3d_button.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int led=0;
int count=0;

int main(void) {
  //uint8_t button_state = 0x00;

  f3d_led_init();
  f3d_button_init();

  //while the button not being pressed
  while(1){
    if(!f3d_button_read()){
      f3d_led_on(led);
      delay();
      f3d_led_off(led);
      led++;
      
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

    //if the button is being pressed, the current led will stay on
    else{
      f3d_led_on(led);
    }
    
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
