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
#include <stdio.h>


volatile int standby_flag = 0;

/*
 * Inits the LSE RTC
 */
void RTC_init(void) {
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
    RTC_InitTypeDef RTC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
    RCC_LSEDriveConfig(RCC_LSEDrive_High);
    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);

    RTC_WriteProtectionCmd(DISABLE); // Disable the RTC's write protection
    RTC_EnterInitMode();
    RTC_WaitForSynchro();

    RTC_StructInit(&RTC_InitStructure);
    RTC_Init(&RTC_InitStructure);

    RTC_DateStructInit(&RTC_DateStructure);
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

    RTC_TimeStructInit(&RTC_TimeStructure);
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

    RTC_ExitInitMode();
    RTC_WriteProtectionCmd(ENABLE);
    PWR_BackupAccessCmd(DISABLE);
}

void RTC_LSI_init(void) {
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
    RTC_InitTypeDef RTC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE); // Enable access to write to the RTC Registers
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);

    PWR_BackupAccessCmd(ENABLE);

    // LSI used as RTC source clock
    RCC_LSICmd(ENABLE);

    // Wait till LSI is ready 
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();


    RTC_StructInit(&RTC_InitStructure); // Set the structure members to their default values
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 88;
    RTC_InitStructure.RTC_SynchPrediv = 470;
    RTC_Init(&RTC_InitStructure);


    RTC_DateStructure.RTC_Year = 13;
    RTC_DateStructure.RTC_Month = RTC_Month_November;
    RTC_DateStructure.RTC_Date = 7;
    RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Thursday;
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours = 0x09;
    RTC_TimeStructure.RTC_Minutes = 0x05;
    RTC_TimeStructure.RTC_Seconds = 0x00;
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

}


/**
 * @brief  This function configures the system to enter Standby mode with RTC
 *         clocked by LSI for current consumption measurement purpose.
 *         STANDBY Mode with RTC clocked by LSI
 *         ========================================
 *           - RTC Clocked by LSI
 *           - IWDG OFF
 *           - Automatic Wakeup using RTC
 * @param  None
 * @retval None
 */

void f3d_enter_standby(void)
{
    /*
    RTC_AlarmTypeDef  RTC_AlarmStructure;
    RTC_DateTypeDef   RTC_CurrentDate;
    RTC_TimeTypeDef   RTC_CurrentTime;

    RTC_GetTime(RTC_Format_BIN,&RTC_CurrentTime);
    RTC_GetDate(RTC_Format_BIN,&RTC_CurrentDate);
    int current_hour = RTC_CurrentTime.Hours;
    
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv = 0x0138;
    

    // Set the alarm X+1m
    if (current_hour < 12)
        RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
    else
        RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;

    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = RTC_CurrentTime.RTC_Hours;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_CurrentTime.RTC_Minutes;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = RTC_CurrentTime.RTC_Seconds;
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_CurrentDate.RTC_Date;
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
    */
    // Configure and Set the wakeup counter
    // Disable RTC Wakeup write protection
    
    // 1. Disable the RTC Write Protection
    RTC_WriteProtectionCmd(DISABLE);

    // 2. Disable the Wakeup Timer
    RTC_WakeUpCmd(DISABLE);

    // 3. Ensure access to Wakeup auto-reload
    while(RTC_GetFlagStatus(RTC_FLAG_WUTWF) == RESET);
    // 4. Program the value into the wakeup timer.
    RTC_SetWakeUpCounter(0xffff);

    // 5. Select the desired clock source.
    //    DIV8 min = 122.07uS (0x0), max = > 131072s (0xffff)
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    // 6. Re-enable the wakeup timer.
    RTC_WakeUpCmd(ENABLE);

    // 7.  Enable the RTC registers' write protection
    RTC_WriteProtectionCmd(ENABLE);
    
    /* Enable NVIC Wakeup Interrupt and Handler */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x07;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable RTC Alarm A Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    // Enable the alarm
    //RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    // Clear Wakeup flag
    PWR_ClearFlag(PWR_FLAG_WU);

    RTC_ClearFlag(RTC_FLAG_WUTF);

    /* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
   // PWR_EnterSTANDBYMode();

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

    PWR_BackupAccessCmd(ENABLE);
    RTC_SetTime(RTC_Format_BIN, &t);
    RTC_SetDate(RTC_Format_BIN, &d);
    PWR_BackupAccessCmd(DISABLE);
    return (0);
}

void RTC_WKUP_IRQHandler(void) {
    standby_flag ^= 1;
}
