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
void f3d_led_hr_display(int hours){
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


void f3d_led_min_display(int mins){
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

void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
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
  int mode=0;
  int set_hr=0;
  int set_min=0;
  int set_unit=1;
  int buzzer=0;
  extern int hold_count;

  //0=not in set mode; 1=in set mode
  int set_mode=0;


  //set the time in serialT. 
  //Hour,Minute,Second,Month,Day,Year

 
  if(f3d_button_read()){
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
  }
}

  
