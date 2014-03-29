/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
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
#include <f3d_button.h>
#include <f3d_uart.h>
#include <f3d_systick.h>
#include <f3d_rtc.h>
#include <schedule.h> 
#include <f3d_delay.h>
#include <ds_nordic.h>
#include <ds_wifibase.h>
#include <nrf24l01.h>
#include <nrf24l01base.h>
#include <stdio.h>

int main(void) { 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_i2c1_init();
  f3d_systick_init();
  RTC_init();
  f3d_delay_init();
  ds_nordic_init();
  ds_wifibase_init();

  /* nrf24l01_initialize_debug(false, 1, false);  // Setup Node at transmitter, Standard Shockburst */
  nrf24l01_initialize_debug(false, 32, true);     // Enhanced Shockburst, Auto Ack turned on
  nrf24l01_clear_flush();

  //   nrf24l01base_initialize_debug(true, 1, false);  // setup wifibase as a receiver, Standard Shockburst
  nrf24l01base_initialize_debug(true, 32, true);       // Enhanced Shockburst, Auto Ack turned on
  nrf24l01base_clear_flush();

  printf("\n#### Begin Wifi Test ####\n");

  // Node Transmission
  char data[32];
  char index;
  for (index=0;index<32;index++) {
    data[index] = 'a'+index;
  }
  printf("Node Data: ");
  for (index=0;index<32;index++) {
    printf("%c",data[index]);
  }
  printf("\n");
   
  printf("Node: transmit character %c\n",data);
  nrf24l01_write_tx_payload(data, 32, true);      // nordic writes a character
  while(!(nrf24l01_irq_pin_active() && (nrf24l01_irq_tx_ds_active() || nrf24l01_irq_max_rt_active()))); // wait until it is gone

  // Node Listen Mode if the maximum retry limit was not hit
  if (!nrf24l01_irq_max_rt_active()) {
    nrf24l01_irq_clear_all();
    nrf24l01_set_as_rx(true);
  }
  else {
    nrf24l01_flush_tx(); //get the unsent character out of the TX FIFO
    nrf24l01_irq_clear_all(); //clear all interrupts
    //    printf("Node: Failed to send %c\n",data);
  } 
  
  // Base Reception
  char rxdata[32];
  while(!(nrf24l01base_irq_pin_active() && nrf24l01base_irq_rx_dr_active()));
  nrf24l01base_read_rx_payload(rxdata, 32);	
  nrf24l01base_irq_clear_all();
  f3d_delay_uS(130);

  // Payload Translation
  printf("Base Data: ");
  for (index=0;index<32;index++) {
    printf("%c",rxdata[index]);
  }
  printf("\n");
  
  for (index=0;index<32;index++) {
    if ((rxdata[index] >= 'a') && (rxdata[index] <= 'z')) {
      rxdata[index]-=32;
    }
  }

  printf("Base: receive character: %c\n",rxdata);
  /* rxdata-=32; */
   printf("Base: upcase: %c\n",rxdata);
    
  // Base Transmission
  printf("Base: Sending Upcase char: %c\n",rxdata);
  nrf24l01base_set_as_tx();
  nrf24l01base_write_tx_payload(rxdata, 32, true);
  while(!(nrf24l01base_irq_pin_active() && (nrf24l01base_irq_tx_ds_active() || nrf24l01base_irq_max_rt_active())));
  nrf24l01base_irq_clear_all();
  nrf24l01base_set_as_rx(true);

  // Node Reception
  char noderx[32];
  while(!(nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active()));
  nrf24l01_read_rx_payload(noderx, 32);

  printf("Node Data: ");
  for (index=0;index<32;index++) {
    printf("%c",noderx[index]);
  }
  printf("\n");

  printf("Node: Receive character %c\n",noderx);
  nrf24l01_irq_clear_all();
  f3d_delay_uS(130);
  nrf24l01_set_as_tx();

  printf("#### End Wifi Test ####\n");

  while (1);
  /*
  while (1) {
    if (systick_flag) {
      systick_flag=0;
      schedule();
    }
  }
  */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
