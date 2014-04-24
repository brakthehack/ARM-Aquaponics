#include "application.h"
#include <stdlib.h>

volatile extern int standby_flag;

#define SEND_INTERVAL_SECONDS 60

int main(){

    app_init();
    
    char txdata[32] = {}, rxdata[32] = {};
    char index;
    int moisture;
    int last_second = 0, current_second = 0;

    RTC_DateTypeDef   RTC_CurrentDate;
    RTC_TimeTypeDef   RTC_CurrentTime;

    RTC_GetDate(RTC_Format_BIN,&RTC_CurrentDate);

    while (1) {
      motor_on();
      printf("%d\n",f3d_read_adc_power());
      /*
        // acquire data
        RTC_GetTime(RTC_Format_BIN,&RTC_CurrentTime);
        current_second = RTC_CurrentTime.RTC_Seconds;
        //app_read_moisture_data(&moisture);
        //printf("moisture %d\n", moisture);
        printf("seconds %d\n", current_second);
        printf("entering standby\n");
        app_enter_stop();
        if (standby_flag) {
            printf("standby triggered\n");
            standby_flag = 0;
            do  {
                RTC_GetTime(RTC_Format_BIN,&RTC_CurrentTime);
                current_second = RTC_CurrentTime.RTC_Seconds;
                
                last_second = current_second;
                app_read_moisture_data(&moisture);
                app_read_moisture_data(&moisture);
                //moisture = rand() % 2000;
                printf("moisture %d\n", moisture);
                app_prepare_nordic_packet(&moisture, txdata);
                //app_print_nordic_data(txdata);
                printf("BEFORE SEND %x%x%x%x\n", txdata[0], txdata[1], txdata[2], txdata[3]);
                app_send_nordic_packet(txdata, rxdata);
                delay(500);
            }while (current_second != 0);

	    }*/
      //printf("after while loop\n");
        f3d_led_all_on();
        delay(200);
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

