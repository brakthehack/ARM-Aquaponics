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
#include <math.h>
#include <stdio.h>




volatile int packet_received = 0;
char rxdata[32];
char txdata[32] = { "Submission received" };
//char index;
int result;
char pump;

volatile int lcd_update=0;
volatile int lcd_update_counter=0;

int m_buffer[10]={0,0,0,0,0,0,0,0,0,0};
//int m_index=9;
int m_update=0;

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

char read_pump_data(char *data){
  char pump=data[4];
  return pump;
}

int read_battery_data(char *data){
  //something

}

void read_rx() {
    nrf24l01_read_rx_payload(rxdata, 32);
    nrf24l01_irq_clear_all();
    f3d_delay_uS(130);

    // Payload Translation
    printf("Base Data: ");
    printf("%d\n",read_moisture_data(rxdata));
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

void lcd_draw_base(){
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawString(10,5,"TIME: ",BLACK,WHITE);

  //drawing chart
  int h,v;
  //the chart is 100x100 pixels
  for(h=15;h<115;h++){
    f3d_lcd_drawPixel(h,120,BLACK);
  }
  for(v=120;v>20;v--){
    f3d_lcd_drawPixel(15,v,BLACK);
  }
  f3d_lcd_drawString(5,115,"L",BLACK,WHITE);
  f3d_lcd_drawString(5,65,"M",BLACK,WHITE);
  f3d_lcd_drawString(5,20,"H",BLACK,WHITE);
  f3d_lcd_drawString(105,125,"NOW",BLACK,WHITE);

  //init pump status will be red
  draw_circle(8,10,150,RED);
  f3d_lcd_drawString(20,145,"PUMP",BLACK,WHITE);

  //battery status drawing
  int i,k;
  for(i=75;i<120;i++){
    f3d_lcd_drawPixel(i,155,BLACK);
  }
  for(i=75;i<120;i++){
    f3d_lcd_drawPixel(i,140,BLACK);
  }
  for(k=140;k<156;k++){
    f3d_lcd_drawPixel(75,k,BLACK);
  }
  for(k=140;k<156;k++){
    f3d_lcd_drawPixel(120,k,BLACK);
  }

  for(i=120;i<125;i++){
    f3d_lcd_drawPixel(i,143,BLACK);
  }
  for(i=120;i<125;i++){
    f3d_lcd_drawPixel(i,152,BLACK);
  }
  for(k=143;k<153;k++){
    f3d_lcd_drawPixel(125,k,BLACK);
  }
}

void pump_draw(char pump){
  //status for water pump; green is on; red is stop
  draw_circle(8,10,150,RED);
}

void battery_color(){
  int x,y;
  for(x=76;x<120;x++){
    for(y=141;y<155;y++){
      f3d_lcd_drawPixel(x,y,GREEN);
    }
  }
  for(x=121;x<125;x++){
    for(y=144;y<152;y++){
      f3d_lcd_drawPixel(x,y,GREEN);
    }
  }
  f3d_lcd_drawString(87,145,"100%",BLACK,GREEN);
}



//moisture from 0-2000,0-600 is LOW,601-1200 is MED, >1600 is HIGH
//the whole graph show ten data,it will pop the oldest one the get the newest data


void graph(int m_buffer[],int flush){
  //x start at 15 to 115
  //y start at 20 to 120
  int i,k,z;
  int ox=20;//15+5
  int oy=15;//20-5
  int dot_y[10];
  float dis;

  /*
  //to flush the grahic before printing the new one
  for(i=16;i<=120;i++){
    for(k=15;k<=119;k++){
      f3d_lcd_drawPixel(i,k,WHITE);
    }
  }
  */

  for(i=0;i<=9;i++){
    dot_y[i]=oy+(100-(int)(m_buffer[i]/20));
    ox+=10;
    oy=15;
  }
  
  //reset ox to start point for the function below
  ox=20;

  //joining dots with line
  for(i=0;i<9;i++){
    //the next one is higher than last one
    if(dot_y[i]-dot_y[i+1]>0){
      for(k=0;k<10;k+=1){
	dis=(dot_y[i]-dot_y[i+1])/10.00;
	if(flush){
	  draw_circle(2,ox+k,dot_y[i]-(dis*k),WHITE);
	}

	else{
	  draw_circle(2,ox+k,dot_y[i]-(dis*k),BLACK);  
	}
      }	  
    }

    else{
      for(k=0;k<10;k+=1){
	dis=(dot_y[i+1]-dot_y[i])/10.00;
	if(flush){
	  draw_circle(2,k+ox,dot_y[i]+(dis*k),WHITE);
	}
	
	else{
	  draw_circle(2,k+ox,dot_y[i]+(dis*k),BLACK);
	}
      }
    }
    ox+=10;
  }

  //reset back to start point for the following function
  ox=20;

  for(i=0;i<=9;i++){
    if(flush){
      draw_circle(4,ox,dot_y[i],WHITE);
    }
    else{
      draw_circle(4,ox,dot_y[i],BLUE);
    }
    ox+=10;
    oy=15;
  }
}

//update the m_buffer if receive a new data, pop the oldest one and push the newest one
void m_buffer_update(int result,int m_buffer[]){
  int i;
  for(i=0;i<9;i++){
    m_buffer[i]=m_buffer[i+1];
  }
  m_buffer[9]=result;
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

    lcd_draw_base();
    battery_color();
    
    
    graph(m_buffer,0);
    //delay(100);
    //m_buffer_update(500,m_buffer);
    //graph(m_buffer);


    while (1) {
      if (packet_received){
            read_rx();
	    m_update=1;
	    result= read_moisture_data(rxdata);
	    graph(m_buffer,1);//for flush the graph
	    m_buffer_update(result,m_buffer);
      }

      if(m_update){
	graph(m_buffer,0);
	m_update=0;
      }





        //printf("%d %d\n", 4 * (sizeof(char)), sizeof(int));
        
	//update lcd every 4 sec
	//if(lcd_update){
	  //f3d_lcd_fillScreen(BLUE);
	  //f3d_lcd_fillScreen(RED);
	  //f3d_lcd_fillScreen(GREEN);
	  //f3d_led_all_on();
	  //lcd_update=0;
	  //lcd_update_counter=0;
	  //f3d_led_all_off();
      //}
       
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
