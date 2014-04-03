#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_button.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <f3d_uart.h>
#include <f3d_dac.h>
#include <f3d_a2d.h>
#include <f3d_timer2.h>
#include <stdio.h>

int main(){

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);


  f3d_led_init();
  f3d_systick_init();
  f3d_uart_init();
  f3d_timer2_init();
  f3d_dac_init();
  f3d_a2d_init();

  while(1){
    printf("%d\n",f3d_read_adc());


  }


}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

