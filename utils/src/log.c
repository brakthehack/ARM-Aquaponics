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
 * Pressure & temp = 2
 * gyro = 1
 */

void get_data(int i) {
  if (i == 1) { // store gyro
    f3d_gyro_getdata(&gyro_buffer[0]);
    if (LOGGING && cacheIndex < LOG_SIZE) {
      gyro_cache[cacheIndex / 10][0] = gyro_buffer[0];
      gyro_cache[cacheIndex / 10][1] = gyro_buffer[1];
      gyro_cache[cacheIndex / 10][2] = gyro_buffer[2];
    }
  }
  if (i == 2) { // store pressure and temp, convert to altitude
    f3d_pressure_write(&ctrl2, 0x21, 1);
    f3d_pressure_getdata(&pressure, &temp);
    convert_to_altitude_ft(&pressure, &altitude);
    if (LOGGING && cacheIndex < LOG_SIZE) {
      altitude_cache[cacheIndex++] = altitude;
    }
  }
}
