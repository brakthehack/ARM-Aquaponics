#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_button.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <f3d_uart.h>
#include <f3d_rtc.h>
#include <f3d_dac.h>
#include <f3d_a2d.h>
#include <f3d_timer2.h>
#include <schedule.h>
#include <ds_wifibase.h>
#include <nrf24l01base.h>
#include <stdio.h>

int main(){

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  
  f3d_led_init();
  f3d_uart_init();
  f3d_systick_init();
  f3d_i2c1_init();
  RTC_init();
  f3d_timer2_init();
  f3d_dac_init();
  f3d_a2d_init();

  //ds_wifibase_init();

  /*
  // nrf24l01base_initialize_debug(true, 1, false); // setup wifibase as a receiver, Standard Shockburst
  nrf24l01base_initialize_debug(true, 32, true); // Enhanced Shockburst, Auto Ack turned on
  nrf24l01base_clear_flush();

  char data[32];
  char index;
  for (index=0;index<32;index++) {
    data[index] = 'a'+index;
  }

  
  printf("\n#### Begin Wifi Test ####\n");
  // Base Transmission
  printf("Base: Sending Upcase char: %c\n",data);
  nrf24l01base_set_as_tx();
  nrf24l01base_write_tx_payload(data, 32, true);
  while(!(nrf24l01base_irq_pin_active() && (nrf24l01base_irq_tx_ds_active() || nrf24l01base_irq_max_rt_active())));
  nrf24l01base_irq_clear_all();
  nrf24l01base_set_as_rx(true);
  */

  while(1){
    //printf("%d\n",f3d_read_adc());
    printf("a\n");

  }

  
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

