#ifndef __RTC_H__
#define __RTC_H__

extern uint16_t date_now[];
void rtc_init();
void rtc_set_time();
void rtc_read_time();

#endif