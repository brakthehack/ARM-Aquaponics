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
#include <schedule.h>
#include <ds_nordic.h>
#include <nrf24l01.h>
#include <stdio.h>

/*
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

*/

volatile int packet_received = 0;
char rxdata[32];
char txdata[32] = { "Submission received" };
char index;

int read_moisture_data(char *data) {
    int result = 0;
    result |= (int) data[0];
    result = result << 8;
    result |= (int) data[1];
    result = result << 8;
    result |= (int) data[2];
    result = result << 8;
    result |= (int) data[3];
    return result;
}

void read_rx() {
    nrf24l01_read_rx_payload(rxdata, 32);
    nrf24l01_irq_clear_all();
    f3d_delay_uS(130);

    // Payload Translation
    printf("Base Data: ");
    printf("%d\n", read_moisture_data(rxdata));
    /*
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
    
    printf("Base Sending uppercase char: %c\n", rxdata);
    */
    printf("Sending %s\n", txdata);
    nrf24l01_set_as_tx();
    nrf24l01_write_tx_payload(txdata, 32, true);
    while(!(nrf24l01_irq_pin_active() && (nrf24l01_irq_tx_ds_active() || nrf24l01_irq_max_rt_active())));
    nrf24l01_irq_clear_all();
    nrf24l01_set_as_rx(true);
    nrf24l01_clear_flush();

}

int main() { 
    f3d_delay_init();
    f3d_delay_uS(10);
    f3d_uart_init();
    f3d_delay_uS(10);
    f3d_led_init();
    f3d_delay_uS(10);
    f3d_lcd_init();
    f3d_delay_uS(10);
    f3d_systick_init();
    f3d_delay_uS(10);
    f3d_i2c1_init();
    f3d_delay_uS(10);
    ds_nordic_init();
    f3d_delay_uS(10);
    nrf24l01_initialize_debug(true, 32, true); // Enhanced Shockburst, Auto Ack turned on
    nrf24l01_clear_flush();

    while (1) {
        if (packet_received)
            read_rx();

        f3d_led_all_on();
        printf("%d %d\n", 4 * (sizeof(char)), sizeof(int));
        f3d_lcd_fillScreen(BLUE);
        f3d_lcd_fillScreen(RED);
        f3d_lcd_fillScreen(GREEN);
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


/* main.c ends here */
