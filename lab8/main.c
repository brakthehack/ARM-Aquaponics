#include <stm32f30x.h>
#include <f3d_rtc.h>
#include <f3d_buzzer.h>
#include <f3d_uart.h>
#include <f3d_systick.h>
#include <f3d_button.h>
#include <f3d_led.h>
#include <f3d_sdcard.h>
#include <f3d_gyro.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <nrf24l01.h>
#include <ds_nordic.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif


int main(void) {

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    f3d_led_init();
    f3d_uart_init();
    f3d_systick_init();
    f3d_button_init();
    f3d_gyro_init();
    f3d_delay_init();
    f3d_i2c1_init();
    f3d_delay_uS(1000);
    f3d_accel_init();
    f3d_delay_uS(1000);
    f3d_mag_init();
    f3d_delay_uS(1000);
    ds_nordic_init();

    bool rx = false;
    unsigned char p0_payload_width;
    bool enable_auto_ack;

    unsigned char txdata[32]={};
    unsigned char rxdata[32]={};
    unsigned char data;

    while(1){
       nrf24l01_clear_flush();

       nrf24l01_write_tx_payload(txdata, 32, true);      
       while(!(nrf24l01_irq_pin_active() && (nrf24l01_irq_tx_ds_active() || nrf24l01_irq_max_rt_active()))); 

       if (!nrf24l01_irq_max_rt_active()) {
	 nrf24l01_irq_clear_all();
	 nrf24l01_set_as_rx(true);
       }
       else {
	 nrf24l01_flush_tx(); //get the unsent character out of the TX FIFO
	 nrf24l01_irq_clear_all(); //clear all interrupts
	 printf("Node: Failed to send %c\n",data);
       } 
    }

    while(!(nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active()));
    nrf24l01_read_rx_payload(rxdata, 32);
    nrf24l01_irq_clear_all();
    
    ds_delay_uS(130);
    nrf24l01_set_as_tx();

}