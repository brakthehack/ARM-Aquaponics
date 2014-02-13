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

#define LOGGING 1 // toggle logging to save space on stm
#define LOG_SIZE 330
#define GYRO_DATA 1
#define PRESSURE_DATA 2

// buffers here
float pressure, temp, altitude, gyro_buffer[2];
#if LOGGING
float altitude_cache[LOG_SIZE / 10];
float gyro_cache[LOG_SIZE][2]; // REMEMBER TO CHANGE THIS IF SYSTIC CHNGS
                                    // also need to change in log_data in cfile
void log_data(int i);
#endif

void get_data(int i);
