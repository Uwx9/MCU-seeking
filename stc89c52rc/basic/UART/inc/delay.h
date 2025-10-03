#ifndef _DELAY_H_
#define _DELAY_H_
#include <stdint.h>

void delay_100ms(void); //@12MHz
void Delay1ms(uint32_t ms);	//@11.0592MHz
void delay(unsigned int t);
#endif