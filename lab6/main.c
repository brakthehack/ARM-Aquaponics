#include <stm32f30x.h>
#include <f3d_rtc.h>
#include <f3d_buzzer.h>
#include <f3d_uart.h>
#include <f3d_systick.h>
#include <f3d_button.h>
#include <f3d_led.h>
#include <f3d_sdcard.h>
#include <f3d_gyro.h>
#include <diskio.h>
#include <math.h>
#include <string.h>

#include <log.h>
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



/* Fat File System Info */
FATFS Fatfs;
FIL Fil;
BYTE Buff[128];



/* Needed as global for alarm utils and logger */
int current_hr = 0, current_min = 0, seconds;
int current_day = 0, current_month = 0, current_year = 0;
char char_buffer[50];

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
    f3d_gyro_init();
    f3d_timer4_init();
    f3d_delay_init();
    f3d_sdcard_init();
    
    /*  Provide a delay to allow the SDCARD time to go through it's power
	up initialization. */
    int o;
    for (o=0;o<500;o++) { 
     f3d_delay_uS(500);
    } 
    
    /* LOGGING DATA */

    extern const char *log_buffer[LOG_SIZE][50];
    extern const char *event[10];
    extern float pressure, temp, altitude, gyro_buffer[2];
    extern uint16_t log_index;

    int count=0;
    char linebuffer[100], field[30];
    char *p = field;
    char delimiter[] = ",";
    int i, field_num;

    // date time
    int button_state=0, mode=0;
    int set_hr=0, set_min = 0, set_unit=1, buzzer=0;
    extern int hold_count, hold_count2;
    extern uint32_t delay_count;
    //0=not in alarm clock set mode
    int set_mode=0;

    /* File System vars */

    //FRESULT rc;
    //UINT bw;

    FRESULT rc;	/* Result code */
    DIR dir;	/* Directory object */
    FILINFO fno;	/* File information object */
    UINT bw, br;
    unsigned int retval;

    f_mount(0, &Fatfs); // register volume work area
  


    /* set the time in serialT using CSV
     * Hour,Minute,Second,Month,Day,Year
     * if button is not pressed the value from LSE rtc
     * will be used
     */

    // get date/time on start up
    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN,&RTC_DateStructure);
    current_hr=RTC_TimeStructure.RTC_Hours;
    current_min=RTC_TimeStructure.RTC_Minutes;
    current_day=RTC_DateStructure.RTC_Date;
    current_month=RTC_DateStructure.RTC_Month;
    current_year=RTC_DateStructure.RTC_Year + 2000;

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){ // make the call directly to avoid delay on startup
        RTC_init();
        printf("RTC Code Start\n");
        printf("@\n");

        getline(linebuffer);
        printf("\nHere is the line: %s\n",linebuffer);
        printf("#\n");

        rtc_settime(linebuffer);
        get_timestamp(current_hr,
                current_min,
                seconds,
                current_day,
                current_month,
                current_year, char_buffer);
        log_data(event[BUTTON_PRESS], char_buffer);
    }

    f3d_led_all_off();
    f3d_frequency(buzzer); // set the buzzer to off

    
      

    

    while (1) {
      
     
      

      if (delay_count % 1000 == 0) {
            if (log_index != 0) {
	      bw=dump_log_to_disk(&Fil, bw);
            }
            log_index = 0;
        }
        if (delay_count % 100 == 0) { // poll the clock less frequently
            RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);
            RTC_GetDate(RTC_Format_BIN,&RTC_DateStructure);
            f3d_gyro_getdata(gyro_buffer);

            // if the board moves more than 10 degrees on any access log it
            if (gyro_buffer[0] > 10 || gyro_buffer[1] > 10 || gyro_buffer[2] > 10) {
                get_timestamp(
                        current_hr,
                        current_min,
                        seconds,
                        current_day,
                        current_month,
                        current_year,
                        char_buffer);
                log_data(event[GYRO_MOVE], char_buffer);
		char buffer[50];
		sprintf(buffer, "x: %f, y: %f, z: %f\n", gyro_buffer[0],
			gyro_buffer[1], gyro_buffer[2]);
		log_gyro(buffer);
            }
            current_hr=RTC_TimeStructure.RTC_Hours;
            current_min=RTC_TimeStructure.RTC_Minutes;
            if (current_hr == 0x0 && current_min == 0) {
                current_day=RTC_DateStructure.RTC_Date;
                current_month=RTC_DateStructure.RTC_Month;
                current_year=RTC_DateStructure.RTC_Year + 2000;
            }
            if (seconds!=RTC_TimeStructure.RTC_Seconds) {
                printf("%02d:%02d:%02d ",
                        RTC_TimeStructure.RTC_Hours,
                        RTC_TimeStructure.RTC_Minutes,
                        RTC_TimeStructure.RTC_Seconds);

                seconds=RTC_TimeStructure.RTC_Seconds;
                printf("%d/%d/%d\n",
                        RTC_DateStructure.RTC_Month,
                        RTC_DateStructure.RTC_Date,
                        RTC_DateStructure.RTC_Year + 2000);
            }
        }
        if(f3d_button_read()){
            button_state ^= 1;
            get_timestamp(current_hr,
                    current_min,
                    seconds,
                    current_day,
                    current_month,
                    current_year, char_buffer);

            log_data(event[BUTTON_PRESS], char_buffer);
        }

        // LED DISPLAYS
        if(button_state)
            f3d_led_min_display(current_min);
        else
            f3d_led_hr_display(current_hr);

        if(hold_count>=300){
            mode=1;
            f3d_led_all_off();
            f3d_led_on(5);
            f3d_led_on(6);
            f3d_led_on(7);
            delay(100);
            f3d_led_all_off();
        }
        if(hold_count2 >= 300){ // set the time in the program
            RTC_init();
            printf("RTC Code Start\n");
            printf("@\n");

            getline(linebuffer);
            printf("\nHere is the line: %s\n",linebuffer);
            printf("#\n");

            rtc_settime(linebuffer);


            get_timestamp(current_hr,
                    current_min,
                    seconds,
                    current_day,
                    current_month,
                    current_year, char_buffer);

            log_data(event[START_SET_TIME], char_buffer);
        }

        //inside the set mode
        if (mode) {
            get_timestamp(current_hr,
                    current_min,
                    seconds,
                    current_day,
                    current_month,
                    current_year, char_buffer);

            log_data(event[SOUND_ALARM], char_buffer);
            printf("going into set mode\n");
            hold_count = 0;
        }
        while(mode){
	  ///////////
	  RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);
	   current_hr=RTC_TimeStructure.RTC_Hours;
	   current_min=RTC_TimeStructure.RTC_Minutes;
	   seconds=RTC_TimeStructure.RTC_Seconds;
	  set_alarm(&set_unit, &set_hr, &set_min); 
            // exit the set mode
            if(hold_count>=100){
                get_timestamp(current_hr,
                        current_min,
                        seconds,
                        current_day,
                        current_month,
                        current_year, char_buffer);
                log_data(event[QUIET_ALARM], char_buffer);

                printf("Exiting set mode\n");
                f3d_led_all_on();
                mode=0;
                delay(100);
                f3d_led_all_off();
                break;
            }
        }

        //check hr and min is same as our setting time, if yes, then buzzer goes off
        if(set_hr==RTC_TimeStructure.RTC_Hours && 
                set_min==RTC_TimeStructure.RTC_Minutes &&
                0x0==RTC_TimeStructure.RTC_Seconds){
            if (!buzzer) {
                get_timestamp(current_hr,
                        current_min,
                        seconds,
                        current_day,
                        current_month,
                        current_year, char_buffer);
                log_data(event[SOUND_ALARM], char_buffer);

                buzzer = 4000;
                f3d_frequency(buzzer);
            }
        }

        if(buzzer&&f3d_button_read()){
            buzzer=0;
            //set_hr=-1; keep alarm set until changed
            //set_min=-1;
            f3d_frequency(buzzer);
            get_timestamp(current_hr,
                    current_min,
                    seconds,
                    current_day,
                    current_month,
                    current_year, char_buffer);
            log_data(event[BUTTON_PRESS], char_buffer);

            get_timestamp(current_hr,
                    current_min,
                    seconds,
                    current_day,
                    current_month,
                    current_year, char_buffer);
            log_data(event[QUIET_ALARM], char_buffer);

            delay(200); // try to avoid the buzzer getting called twice
        } 
    }
}
