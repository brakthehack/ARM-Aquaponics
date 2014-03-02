/* alarm_clock_utils.c ---
 *
 * Filename: alarm_clock_utils.c
 * Description:
 * Author: Ching Yuen Ng/ Brian Rak
 * Maintainer:
 * Created: Sat Mar 1 2014
 * Last-Updated: 03/01/2014
 *           By: brak/ching
 *     Update #: 1
 * Keywords:
 * Compatibility:
 *
 */

/* Display the minutes on the LED
*/

#include <alarm_clock_utils.h>
#include <f3d_led.h>

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

/* Function for right hand side (minutes)
 * led0 = 1, led1 = 2, led2 = 4, led3 = 8
 */
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

/*
 * Display the hours on the LEDs
 * Quadrant on left side of LEDs, Binary rep in hour_right()
 */
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

/* for the right hand side of the leds to display hours
 * led0 = 1, led1 = 2, led2 = 4, led3 = 8
 */
void hour_right(int n){
    if(n==1){f3d_led_on(0);}
    else if(n==2){f3d_led_on(1);}
    else if(n==3){f3d_led_on(0);f3d_led_on(1);}
    else if(n==4){f3d_led_on(2);}
    else if(n==5){f3d_led_on(0);f3d_led_on(2);}
}

/* Enters the set alarm mode (as requested by the user)
 * Note: the default implementation is f3d_button_read
 */
void set_alarm(int *set_unit, int *set_hr, int *set_min, int *hold_count) {
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
    // hour mode
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
    // exit the set mode
    if(hold_count>=200){
        mode=0;
        f3d_led_all_off();
        f3d_led_on(1);
        f3d_led_on(2);
        f3d_led_on(3);
        f3d_led_all_off();
        break;
    }


}

void read_time(TimeStruct *time) {

}
