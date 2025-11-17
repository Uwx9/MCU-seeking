#include "stm32f10x.h"
#include <time.h>

uint16_t date_now[] = {2025, 11, 15, 19, 35, 0};

void rtc_set_time()
{
	time_t time_cnt;
	struct tm time_date;
	time_date.tm_year = date_now[0] - 1900;
	time_date.tm_mon = date_now[1] - 1;
	time_date.tm_mday = date_now[2];
	time_date.tm_hour = date_now[3];
	time_date.tm_min = date_now[4];
	time_date.tm_sec =  date_now[5];

	time_cnt = mktime(&time_date) - 8 * 60 * 60;	// 我们在东八区
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();
}

void rtc_read_time()
{
	time_t time_cnt;
	struct tm time_date;

	time_cnt = RTC_GetCounter() + 8 * 60 * 60;
	time_date = *localtime(&time_cnt);
	date_now[0] = time_date.tm_year + 1900;
	date_now[1] = time_date.tm_mon + 1;
	date_now[2] = time_date.tm_mday;
	date_now[3] = time_date.tm_hour;
	date_now[4] = time_date.tm_min;
	date_now[5] = time_date.tm_sec;
}

void rtc_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd(ENABLE);	// 使能BKP

	if (BKP_ReadBackupRegister(BKP_DR1) != 0xa9a9) {
		RCC_LSEConfig(RCC_LSE_ON);		// 开启LSE
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);	// 等待LSE准备好
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);				// RTCCLK选择LSE
		RCC_RTCCLKCmd(ENABLE);			// RTCCLK使能

		RTC_WaitForSynchro();	// 等待同步
		RTC_WaitForLastTask();	// 等待上次写入完成

		RTC_SetPrescaler(32768 - 1);	// 设置预分频系数
		RTC_WaitForLastTask();

		rtc_set_time();
		RTC_WaitForLastTask();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xa9a9);
	} else {
		RTC_WaitForSynchro();	// 等待同步
		RTC_WaitForLastTask();	// 等待上次写入完成
	}
}
