/**
 * This file provides the interface for the outside stm watering garden.
 */

#include "application.h"

RTC_DateTypeDef   RTC_CurrentDate;
RTC_TimeTypeDef   RTC_CurrentTime;

// sends a packet over the network using the nordics
// converts data to send it over the network if it is not of type char *
void app_send_nordic_packet(char *txdata, char *rxdata) {

    int index;

    printf("\n");
    printf("Node: transmit character %c\n",txdata);
    nrf24l01base_write_tx_payload(txdata, 32, true);      // nordic writes a character
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
        printf("Node: Failed to send %c\n",txdata);
    }

    printf("node reception\n");
    while(!(nrf24l01base_irq_pin_active() && nrf24l01base_irq_rx_dr_active()));
    nrf24l01base_read_rx_payload(rxdata, 32);

    printf("Node Data: ");
    for (index=0;index<32;index++) {
        printf("%c",rxdata[index]);
    }
    printf("\n");

    printf("Node: Receive character %c\n",rxdata);
    nrf24l01base_irq_clear_all();
    f3d_delay_uS(130);
    nrf24l01base_set_as_tx();

}

// Converts int data into a nordic packet to send over the network
void app_prepare_nordic_packet (int *mdata, char *data) {
    
    data[3]= *mdata;
    *mdata= (*mdata)>>8;
    data[2]= *mdata;
    *mdata= (*mdata)>>8;
    data[1]= *mdata;
    *mdata= (*mdata)>>8;
    data[0]= *mdata;
}

// initializes all of the drivers for the peripherals
void app_init(void) {
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
    f3d_timer2_init();
    f3d_delay_uS(10);
    f3d_dac_init();
    f3d_delay_uS(10);
    f3d_a2d_init();
    f3d_delay_uS(10);
    ds_wifibase_init();
    f3d_delay_uS(10);
    f3d_stop_init();
    //RTC_init();
    //RTC_LSI_init();
    motor_init();

    /* nrf24l01base_initialize_debug(false, 1, false); // Setup Node at transmitter, Standard Shockburst */
    nrf24l01base_initialize_debug(false, 32, true); // Enhanced Shockburst, Auto Ack turned on
    nrf24l01base_clear_flush();
}

// reads and stores data into a buffer until full
void app_read_moisture_data(int *data) {
   *data = f3d_read_adc(MOISTURE);
}

// turns on the pump
// has no effect is the pump is running
void app_turn_on_pump(void) {

}

// turns off the pump
// has no effect is the pump is not running
void app_turn_off_pump(void) {

}

// check to see if the pump is turned on
// @return 0 if the pump is NOT on, 1 if it is on
int app_check_pump_status(void) {

}

// reads the power level of the battery
// stores the data into the pointer
void app_read_battery_power(float *data) {

}

void app_enter_stop(void) {
    //f3d_led_all_on();
    RTC_GetTime(RTC_Format_BIN,&RTC_CurrentTime);
    
    printf("Wakeup Counter: %d\n", RTC_GetWakeUpCounter());
    printf("WUT_IT_Status : %d\n",RTC_GetITStatus(RTC_IT_WUT));
    printf("%d:%d:%d\n", RTC_CurrentTime.RTC_Hours,
            RTC_CurrentTime.RTC_Minutes, RTC_CurrentTime.RTC_Seconds);
    printf("Month: %d Year: %d\n", RTC_CurrentDate.RTC_Month,
            RTC_CurrentDate.RTC_Year); 
    
    delay(1000);    
    //f3d_led_all_off();
    //delay(200);

    // Enable Wakeup Counter
    //RTC_WakeUpCmd(ENABLE);

    f3d_led_all_off();
    // Enter Stop Mode
    PWR_EnterSleepMode(PWR_SLEEPEntry_WFI); 
    //app_init2(); 
    f3d_led_all_on();
}

void app_print_nordic_data(char *data) {
    int index;
    for (index=0;index<32;index+=4) {
        data[index] = 'a'+index;
    }
    printf("Node Data: ");
    for (index=0;index<32;index++) {
        printf("%c",data[index]);
    }
}

void app_init2(void) {
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
    f3d_timer2_init();
    f3d_delay_uS(10);
    f3d_dac_init();
    f3d_delay_uS(10);
    f3d_a2d_init();
    f3d_delay_uS(10);
    ds_wifibase_init();
    f3d_delay_uS(10);
    //f3d_stop_init();
    //RTC_init();
    //RTC_LSI_init();
    motor_init();

    /* nrf24l01base_initialize_debug(false, 1, false); // Setup Node at transmitter, Standard Shockburst */
    nrf24l01base_initialize_debug(false, 32, true); // Enhanced Shockburst, Auto Ack turned on
    nrf24l01base_clear_flush();
}

