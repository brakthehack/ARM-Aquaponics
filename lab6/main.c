#include <stm32f30x.h>
#include <f3d_rtc.h>
#include <f3d_buzzer.h>
#include <f3d_uart.h>
#include <f3d_systick.h>
#include <f3d_button.h>
#include <f3d_led.h>
#include <time.h>
#include <alarm_clock_utils.h>

#include <stdio.h>

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif


void getline(char *buf) {
    char c;
    while ((c=getchar())!='\n') {
        *buf++ = c;
        putchar(c);
    }
    *buf = 0;
}

int main(void) {
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    f3d_uart_init();
    f3d_systick_init();
    f3d_led_init();
    f3d_button_init();
    f3d_timer4_init();

    extern uint8_t button_check_state, button_check_state2;
    extern volatile uint16_t button_state, button_state2;
    extern volatile uint16_t time_passed, time_passed2;
    /* Button test code */
       while (1) {
       delay(20);
       printf("U, %d E %d %d\n", 
       f3d_button_state_read(&button_check_state, &button_state, &time_passed),
       f3d_extra_button(),
       f3d_button_state_read(&button_check_state2, &button_state2, &time_passed2));
       } 
    /*
    int count=0;
    int seconds;
    char linebuffer[100];
    char field[30];
    char *p = field;
    char delimiter[] = ",";
    int field_num;
    int i;
    int button_state=0;
    int mode=0;
    int set_hr=0;
    int set_min=0;
    int set_unit=1;
    int buzzer=0;
    extern int hold_count;

    //0=not in alarm clock set mode
    int set_mode=0;

    /* set the time in serialT using CSV
     * Hour,Minute,Second,Month,Day,Year
     * if button is not pressed the value from LSE rtc
     * will be used
     
    if(f3d_button_read()){
        RTC_init();
        printf("RTC Code Start\n");
        printf("@\n");

        getline(linebuffer);
        printf("\nHere is the line: %s\n",linebuffer);
        printf("#\n");

        rtc_settime(linebuffer);
    }

    f3d_led_all_off();

    f3d_frequency(buzzer);



    while (1) {

        RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BIN,&RTC_DateStructure);


        int current_hr=RTC_TimeStructure.RTC_Hours;
        int current_min=RTC_TimeStructure.RTC_Minutes;


        if (seconds!=RTC_TimeStructure.RTC_Seconds) {
            printf("%02d:%02d:%02d ",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
            seconds=RTC_TimeStructure.RTC_Seconds;
            printf("%d/%d/%d\n",RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date,RTC_DateStructure.RTC_Year + 2000);
        }

        if(f3d_button_read()){button_state^=1;}

        if(!button_state){f3d_led_hr_display(current_hr);}

        if(button_state){f3d_led_min_display(current_min);}

        if(hold_count>=400){
            mode=1;
            f3d_led_all_off();
            f3d_led_on(5);
            f3d_led_on(6);
            f3d_led_on(7);
            delay();
            f3d_led_all_off();
        }

        //inside the set mode
        while(mode==1){
            //f3d_led_all_off();
            printf("in set mode\n");
            //delay();

            if(f3d_button_read()){
                set_unit^=1;
                if(set_unit==0){
                    f3d_led_all_on();
                    delay();
                    f3d_led_all_off();
                    printf("hr\n");
                }
                else{
                    f3d_led_all_on();
                    delay();
                    f3d_led_all_off();
                    delay();
                    f3d_led_all_on();
                    delay();
                    f3d_led_all_off();
                    printf("min\n");
                }

            }

            //will go into set hour mide first
            if(set_unit==0){
                if(f3d_extra_button()){
                    set_hr++;
                    f3d_led_hr_display(set_hr);
                }
            }

            if(set_unit==1){
                if(f3d_extra_button()){
                    set_min++;
                    f3d_led_min_display(set_min);
                }
            }

            //to exit the set mode
            if(hold_count>=200){
                mode=0;
                f3d_led_all_off();
                f3d_led_on(1);
                f3d_led_on(2);
                f3d_led_on(3);
                f3d_led_all_off();
                break;
            }
            delay();
        }


        //check hr and min is same as our setting time, if yes, then buzzer goes off
        if(set_hr==RTC_TimeStructure.RTC_Hours&&set_min==RTC_TimeStructure.RTC_Minutes){
            if (!buzzer) {
                buzzer = 4000;
                f3d_frequency(buzzer);
            }
        }

        if(buzzer&&f3d_button_read()){
            buzzer=0;
            set_hr=0;
            set_min=0;
            f3d_frequency(buzzer);
        } 
    }*/
}


