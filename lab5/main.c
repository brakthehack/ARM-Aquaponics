#include <stm32f30x.h>
#include <f3d_rtc.h>
#include <f3d_buzzer.h>
#include <f3d_uart.h>
#include <f3d_systick.h>
#include <f3d_button.h>
#include <f3d_led.h>
#include <time.h>

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

//for the right hand side of the leds to display hours
//led0=1,led1=2,led2=4,led3=8
void hour_right(int n){
  if(n==1){f3d_led_on(0);}
    else if(n==2){f3d_led_on(1);}
    else if(n==3){f3d_led_on(0);f3d_led_on(1);}
    else if(n==4){f3d_led_on(2);}
    else if(n==5){f3d_led_on(0);f3d_led_on(2);}
}

//for the entire led to display hours
//left hand side is for thr quadrant and use Hour_right function for right hand side
void f3d_led_hr_display(RTC_TimeTypeDef rtc){
  int hours=rtc.RTC_Hours;
  f3d_led_all_off();

  if(hours<=5){
    f3d_led_on(7);
    hour_right(hours);
  }
  else if(hours<=11){
    f3d_led_on(6);
    hours=hours-6;
    hour_right(hours);
  }
  else if(hours<=17){
    f3d_led_on(5);
    hours=hours-12;
    hour_right(hours);
  }
  else if(hours<=23){
    f3d_led_on(4);
    hours=hours-18;
    hour_right(hours);
  }

}

//for the right hand side of minutues
//led0=1,led1=2,led2=4,led3=8
void min_right(int n){
  if(n==1){f3d_led_on(0);}
  else if(n==2){f3d_led_on(1);}
  else if(n==3){f3d_led_on(0);f3d_led_on(1);}
  else if(n==4){f3d_led_on(2);}
  else if(n==5){f3d_led_on(0);f3d_led_on(2);}
  else if(n==6){f3d_led_on(1);f3d_led_on(2);}
  else if(n==7){f3d_led_on(0);f3d_led_on(1);f3d_led_on(2);}
  else if(n==8){f3d_led_on(3);}
  else if(n==9){f3d_led_on(0);f3d_led_on(3);}
  else if(n==10){f3d_led_on(1);f3d_led_on(3);}
  else if(n==11){f3d_led_on(0);f3d_led_on(1);f3d_led_on(3);}
  else if(n==12){f3d_led_on(2);f3d_led_on(3);}
  else if(n==13){f3d_led_on(0);f3d_led_on(2);f3d_led_on(3);}
  else if(n==14){f3d_led_on(1);f3d_led_on(2);f3d_led_on(3);}
  else if(n==15){f3d_led_on(0);f3d_led_on(1);f3d_led_on(2);f3d_led_on(3);}
}


void f3d_led_min_display(RTC_TimeTypeDef rtc){
  int mins=rtc.RTC_Minutes;
  f3d_led_all_off();

   if(mins<15){
    f3d_led_on(7);
    min_right(mins);
  }
  else if(mins<30){
    f3d_led_on(6);
    mins=mins-15;
    min_right(mins);
  }
  else if(mins<45){
    f3d_led_on(5);
    mins=mins-30;
    min_right(mins);
  }
  else if(mins<60){
    f3d_led_on(4);
    mins=mins-45;
    min_right(mins);
  }
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
  RTC_init();
  f3d_timer4_init();
  

  int count=0;
  int seconds;
  char linebuffer[100];
  char field[30];
  char *p = field;
  char delimiter[] = ",";
  int field_num;
  int i;
  int button_state=0;

  //set the time in serialT. 
  //Hour,Minute,Second,Month,Day,Year
  printf("RTC Code Start\n");
  printf("@\n");
  
  getline(linebuffer);
  printf("\nHere is the line: %s\n",linebuffer);
  printf("#\n");
 
  rtc_settime(linebuffer);
  

  f3d_led_all_off();
  
  while (1) {
    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN,&RTC_DateStructure);
    
    if (seconds!=RTC_TimeStructure.RTC_Seconds) {
      printf("%02d:%02d:%02d ",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
      seconds=RTC_TimeStructure.RTC_Seconds;
      printf("%d/%d/%d\n",RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date,RTC_DateStructure.RTC_Year + 2000);
    }

    if(f3d_button_read()){button_state^=1;}
   
    if(!button_state){f3d_led_hr_display(RTC_TimeStructure);}

    if(button_state){f3d_led_min_display(RTC_TimeStructure);}
    

  }
}

  


  /*
    LSE_init();
    printf("LSE Code Start\n");
    RTC_init();
    
    printf("RTC Code Start\n");
    printf("@\n");
    getline(linebuffer);
    printf("#\n");
    rtc_settime(linebuffer);
  */
 

