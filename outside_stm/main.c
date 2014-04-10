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

    /* nrf24l01base_initialize_debug(false, 1, false);  // Setup Node at transmitter, Standard Shockburst */
    nrf24l01base_initialize_debug(false, 32, true);     // Enhanced Shockburst, Auto Ack turned on
    nrf24l01base_clear_flush();

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
    nrf24l01base_write_tx_payload(data, 32, true);      // nordic writes a character
    while(!(nrf24l01base_irq_pin_active() && (nrf24l01base_irq_tx_ds_active() || nrf24l01base_irq_max_rt_active()))); // wait until it is gone

    //Node Listen Mode if the maximum retry limit was not hit
    if (!nrf24l01base_irq_max_rt_active()) {
        nrf24l01base_irq_clear_all();
        nrf24l01base_set_as_rx(true);
        printf("true\n");
    }
    else {
        nrf24l01base_flush_tx(); //get the unsent character out of the TX FIFO
        nrf24l01base_irq_clear_all(); //clear all interrupts
        printf("Node: Failed to send %c\n",data);
    }

    // Node reception

    printf("node reception\n");
    char noderx[32];
    while(!(nrf24l01base_irq_pin_active() && nrf24l01base_irq_rx_dr_active()));
    nrf24l01base_read_rx_payload(noderx, 32);

    printf("Node Data: ");
    for (index=0;index<32;index++) {
        printf("%c",noderx[index]);
    }
    printf("\n");

    printf("Node: Receive character %c\n",noderx);
    nrf24l01base_irq_clear_all();
    f3d_delay_uS(130);
    nrf24l01base_set_as_tx();


    while(1){
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

