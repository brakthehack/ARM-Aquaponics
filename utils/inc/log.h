/* log.h --- 
 * 
 * Filename: log.h
 * Description: logging utilties for stm32f30x periphs
 * Author: 
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Wed 2-11-14
 * Last-Updated: 02-11-14
 *           By: Ching Yuen Ng/ Brian Rak
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

#include <stm32f30x.h>

#define LOG_SIZE 330
#define GYRO_DATA 1
#define PRESSURE_DATA 2

// buffers here
uint8_t ctrl2=0x01;
float pressure, temp, altitude, gyro_buffer[2];

static uint8_t cacheIndex = 0;
float altitude_cache[LOG_SIZE];
float gyro_cache[LOG_SIZE / 10][2]; // REMEMBER TO CHANGE THIS IF SYSTIC CHNGS

void get_data(int i);
