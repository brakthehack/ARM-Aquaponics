#include <stm32f30x.h>
#include <f3d_rtc.h>
#include <f3d_buzzer.h>
#include <f3d_uart.h>
#include <f3d_systick.h>
#include <f3d_button.h>
#include <f3d_led.h>
#include <f3d_sdcard.h>
#include <f3d_gyro.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif

int main(void) {

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    f3d_led_init();
    f3d_uart_init();
    f3d_systick_init();
    f3d_button_init();
    f3d_gyro_init();
    f3d_delay_init();
    f3d_i2c1_init();
    f3d_delay_uS(1000);
    f3d_accel_init();
    f3d_delay_uS(1000);
    f3d_mag_init();
    f3d_delay_uS(1000);

    float mag_data[3];
    float accel_data[3];

    uint8_t button_state = 0;

    while(1) {
        f3d_mag_read(mag_data);
        f3d_accel_read(accel_data);
        delay(100);
        button_state ^= f3d_button_read();

        //printf("Accel: %f, %f, %f\n", accel_data[0], accel_data[1], accel_data[2]);
        float compass = f3d_calculate_direction(accel_data, mag_data);
        int i;
        printf("Compass Direction: %f\n", compass);
        if (button_state) {
            for (i = 0; i < 8; i++) {
                if (compass < (i + 1) * 45) {
                    f3d_led_all_off();
                    f3d_led_on(i);
                    break;
                }
            } 
        } else {
            for (i = 0; i < 8; i++) {
                if (compass < (i + 1) * 45) {
                    f3d_led_all_off();
                    f3d_led_on(7 - i);
                    break;
                }
            }
        }
    }
}

