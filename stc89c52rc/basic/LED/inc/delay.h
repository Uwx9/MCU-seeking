#ifndef _DELAY_H_
#define _DELAY_H_
#include <stdint.h>

void delay_100ms(void); //@12MHz
void delay_ms(uint16_t ms);
void delay(unsigned int t);
#endif