/* log.c --- 
 * 
 * Filename: log.c
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


#include <log.h>
#include <f3d_gyro.h>
#include <f3d_pressure.h>
#include <pressure_utils.h>

/*
 * get_data takes an int to check what kind of data needs to
 * be put into the buffer
 * Gyro = 1
 * Pressure & temp = 2
 * ALL = 3
 */

uint8_t altIndex = 0;  // current log index for altitude
uint8_t gyroIndex = 0;  // current log index for gyro

void get_data(int i) {
  if (i & 1) { // store gyro
    f3d_gyro_getdata(&gyro_buffer[0]);
  }
  if ((i >> 1) & 1) { // store pressure and temp, convert to altitude
    uint8_t ctrl2=0x01;
    f3d_pressure_write(&ctrl2, 0x21, 1);
    f3d_pressure_getdata(&pressure, &temp);
    convert_to_altitude_ft(&pressure, &altitude);
  }
}

#if LOGGING
void log_data(int i) {
   if ((gyroIndex < LOG_SIZE) && (i & 1)) {
      gyro_cache[gyroIndex][0] = gyro_buffer[0];
      gyro_cache[gyroIndex][1] = gyro_buffer[1];
      gyro_cache[gyroIndex++][2] = gyro_buffer[2];
    }
    if ((altIndex < LOG_SIZE / 10) && ((i >> 1) & 1)) {
      altitude_cache[altIndex++] = altitude;
    }
}
#endif
