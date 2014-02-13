/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 02-2-2014
 *           By: Ching Yuen Ng/ Brian Rak
 *     Update #: 2
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
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_button.h>
#include <f3d_gyro.h>
#include <stdio.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

int main(void) { 
  f3d_uart_init();
  f3d_led_init();
  f3d_button_init();
  f3d_gyro_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  int button;
  float buffer[2];
  int xyz_axis[]={1,0,0};
  char c;

  while (1) {
    //read button state
    button=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);

    //read char input from keyboard
    c=readchar();

    //read the char to change the axis
    if(c=='x') {
      xyz_axis[0]=1;
      xyz_axis[1]=0;
      xyz_axis[2]=0;
    }
    else if(c=='y') {
      xyz_axis[0]=0;
      xyz_axis[1]=1;
      xyz_axis[2]=0;
    }
    else if(c=='z') {
      xyz_axis[0]=0;
      xyz_axis[1]=0;
      xyz_axis[2]=1;
    }

    //if the button is pressed, change to next axis
    if(button){
      if(xyz_axis[0]){
	xyz_axis[0]=0;
	xyz_axis[1]=1;
	xyz_axis[2]=0;
      }
      else if(xyz_axis[1]){
	xyz_axis[0]=0;
	xyz_axis[1]=0;
	xyz_axis[2]=1;
      }
      else if (xyz_axis[2]){
	xyz_axis[0]=1;
	xyz_axis[1]=0;
	xyz_axis[2]=0;
      }
    }

    //printf("x: %f y: %f z: %f\n", buffer[0], buffer[1], buffer[2]);

    //get data from gyro
    f3d_gyro_getdata(buffer);
    assert_failed(&f3d_gyro_getdata, 113); 
    //print out the current axis and the value of it
    if(xyz_axis[0]){printf("Currently in X Axis: %f\n",buffer[0]);}
    else if(xyz_axis[1]){printf("Currently in Y Axis: %f\n",buffer[1]);}
    else if(xyz_axis[2]){printf("Currently in Z Axis: %f\n",buffer[2]);}

    //to get the rate of current axis
    int rate,i;    
    for(i=0;i<3;i++){
	if(xyz_axis[i]){
	  rate=buffer[i];
	  break;
	}
    }

    //LEDs start from all off
    f3d_led_all_off();
        
    //start from North LED, positive value ligth up western side 
    if(rate>5||rate<-5) f3d_led_on(0); //North LED
    if(rate>75) f3d_led_on(7);
    if(rate>150) f3d_led_on(6);
    if(rate>225) f3d_led_on(5);

    //start from North LED, negative value light up eastern side
    if(rate>300||rate<-300) f3d_led_on(4); //South LED
    if(rate<-75) f3d_led_on(1);
    if(rate<-150) f3d_led_on(2);
    if(rate<-225) f3d_led_on(3);
  }


}
/* main.c ends here */
