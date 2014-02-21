/* f3d_rtc.c --- 
 * 
 * Filename: f3d_rtc.c
 * Description: 
 * Author: Ching Yuen Ng/ Brian Rak
 * Maintainer: 
 * Created: Fri Feb 21 2014
 * Last-Updated: 02/21/2014
 *           By: brak/ching
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

#include <f3d_rtc.h>

int LSE_init (void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RCC_LSEDriveConfig(RCC_LSEDrive_Low);
    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
    PWR_BackupAccessCmd(DISABLE);
    return 0;
}

int RTC_init(void) {
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
    RTC_InitTypeDef RTC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    if (RTC_GetFlagStatus(RTC_FLAG_RSF) == SET) {
        printf("Warm Boot\n");
    } else {
        printf("Cold Reset\n");
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
        PWR_BackupAccessCmd(ENABLE);
        RCC_BackupResetCmd(ENABLE);
        RCC_BackupResetCmd(DISABLE);
        RCC_LSEDriveConfig(RCC_LSEDrive_High);
        RCC_LSEConfig(RCC_LSE_ON);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForSynchro();
        RTC_StructInit(&RTC_InitStructure);
        RTC_Init(&RTC_InitStructure);
        RTC_DateStructInit(&RTC_DateStructure);
        RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
        RTC_TimeStructInit(&RTC_TimeStructure);
        RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
        PWR_BackupAccessCmd(DISABLE);
    }
}

