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

int rtc_settime (char *buf) {
    enum {HOUR,MINUTE,SECOND,MONTH,DAY,YEAR};
    RTC_TimeTypeDef t;
    RTC_DateTypeDef d;
    char *tok_start = buf;
    char *tok_end = buf;
    int field = HOUR;
    RTC_TimeStructInit(&t);
    RTC_DateStructInit(&d);
    while (field<=YEAR) {
        while ((*tok_end != ',') && (*tok_end)) {
            tok_end++;
        }
        *tok_end = 0;
        switch (field++) {
            case HOUR:
            t.RTC_Hours=atoi(tok_start);
            printf("hour = %d\n",t.RTC_Hours);
            break;
            case MINUTE:
            t.RTC_Minutes=atoi(tok_start);
            printf("min = %d\n",t.RTC_Minutes);
            break;
            case SECOND:
            t.RTC_Seconds=atoi(tok_start);
            printf("sec = %d\n",t.RTC_Seconds);
            break;
            case MONTH:
            d.RTC_Month=atoi(tok_start);
            printf("month = %d\n",d.RTC_Month);
            break;
            case DAY:
            d.RTC_Date=atoi(tok_start);
            printf("day = %d\n",d.RTC_Date);
            break;
            case YEAR:
            d.RTC_Year=atoi(tok_start) - 2000;
            printf("year = %d\n",d.RTC_Year);
            break;
        }
        tok_end++;
        tok_start=tok_end;
    }
}
