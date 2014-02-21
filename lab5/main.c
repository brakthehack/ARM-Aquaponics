#include <stm32f30x.h>
#include <f3d_rtc.h>
#include <f3d_buzzer.h>
#include <f3d_uart.h>
#include <f3d_systick.h>
#include <f3d_button.h>
#include <f3d_led.h>

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
    //f3d_buzzer_init();
    f3d_timer4_pin();
    f3d_timer4_init();
    
    frequency(4000);

    f3d_systick_init();
    f3d_led_init();

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
    while (1);
}
