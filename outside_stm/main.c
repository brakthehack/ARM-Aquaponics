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

volatile extern int standby_flag;

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
    f3d_timer2_init();
    f3d_delay_uS(10);
    f3d_dac_init();
    f3d_delay_uS(10);
    f3d_a2d_init();
    f3d_delay_uS(10);
    ds_wifibase_init();
    f3d_delay_uS(10);
    //RTC_init();
    //RTC_LSI_init();
    /* nrf24l01base_initialize_debug(false, 1, false);  // Setup Node at transmitter, Standard Shockburst */
    nrf24l01base_initialize_debug(false, 32, true);     // Enhanced Shockburst, Auto Ack turned on
    nrf24l01base_clear_flush();

    char data[32];
    char index;

    //printf("%d\n",f3d_read_adc());

    int mdata=f3d_read_adc();

    printf("\n");
    printf("%d\n",mdata);

    data[3]= mdata;
    mdata=mdata>>8;
    //printf("%x",data[3]);

    data[2]= mdata;
    mdata=mdata>>8;
    data[1]=  mdata;
    mdata=mdata>>8;
    data[0]=  mdata;

    f3d_enter_standby();
    RTC_DateTypeDef   RTC_CurrentDate;
    RTC_TimeTypeDef   RTC_CurrentTime;
    while(1) {

        RTC_GetTime(RTC_Format_BIN,&RTC_CurrentTime);
        RTC_GetDate(RTC_Format_BIN,&RTC_CurrentDate);

        if (standby_flag) {
            printf("Standby triggered\n");
            //while (1);
        }

        f3d_led_all_on();
        printf("Wakeup Counter: %d\n", RTC_GetWakeUpCounter());
        printf("%d:%d:%d\n", RTC_CurrentTime.RTC_Hours,
                RTC_CurrentTime.RTC_Minutes, RTC_CurrentTime.RTC_Seconds);
        printf("Month: %d Year: %d\n", RTC_CurrentDate.RTC_Month,
                RTC_CurrentDate.RTC_Year);
        f3d_led_all_off();
        delay(200);
        
        // Enable Wakeup Counter
        RTC_WakeUpCmd(ENABLE);

        // Enter Stop Mode
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

        // Enable Wakeup Counter
        RTC_WakeUpCmd(DISABLE);

        // After wake-up from STOP reconfigure the system clock
        RCC_LSEConfig(RCC_HSE_ON);

        // Wait till HSE is ready
        while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
        {}

        // Enable PLL
        RCC_PLLCmd(ENABLE);

        // Wait till PLL is ready
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {}

        // Select PLL as system clock source
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        // Wait till PLL is used as system clock source
        while (RCC_GetSYSCLKSource() != 0x0C)
        {}
        
    }
    /*
       for (index=0;index<32;index+=4) {
       data[index] = 'a'+index;
       }
       printf("Node Data: ");
       for (index=0;index<32;index++) {
       printf("%c",data[index]);
       }

*/


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
        //printf("a\n");
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

