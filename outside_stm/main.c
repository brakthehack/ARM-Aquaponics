#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_button.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <f3d_uart.h>
#include <f3d_rtc.h>
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

    f3d_delay_init();
    f3d_delay_uS(10);
    f3d_led_init();
    f3d_delay_uS(10);
    f3d_uart_init();
    f3d_delay_uS(10);
    f3d_systick_init();
    f3d_delay_uS(10);
    f3d_i2c1_init();
    f3d_delay_uS(10);
    RTC_init();
    f3d_delay_uS(10);
    f3d_timer2_init();
    f3d_delay_uS(10);
    f3d_dac_init();
    f3d_delay_uS(10);
    f3d_a2d_init();
    f3d_delay_uS(10);
    ds_wifibase_init();
    f3d_delay_uS(10);

    

    //   nrf24l01base_initialize_debug(true, 1, false);  // setup wifibase as a receiver, Standard Shockburst
    nrf24l01base_initialize_debug(true, 32, true);       // Enhanced Shockburst, Auto Ack turned on
    nrf24l01base_clear_flush();
    printf("Begin wifi test\n");
    // Base Reception
    char rxdata[32];
    char index;
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
    // rxdata-=32; 
    printf("Base: upcase: %c\n",rxdata);

 

    
    // Base Transmission
    printf("Base: Sending Upcase char: %c\n",rxdata);
    nrf24l01base_set_as_tx();
    nrf24l01base_write_tx_payload(rxdata, 32, true);
    while(!(nrf24l01base_irq_pin_active() && (nrf24l01base_irq_tx_ds_active() || nrf24l01base_irq_max_rt_active())));
    nrf24l01base_irq_clear_all();
    nrf24l01base_set_as_rx(true);

    
    while(!){
      //printf("%d\n",f3d_read_adc());
        printf("a\n");
        f3d_led_all_on();
        f3d_delay_uS(100);
        f3d_led_all_off();
    }


}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif

