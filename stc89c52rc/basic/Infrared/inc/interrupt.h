#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_
#include <stdint.h>

// 从左到右依次赋值, 总的括号里的值是最后一个子表达式的值
#define T0_set_initvalue(value) (TH0 = value / 256, TL0 = value % 256)	

#define T0_open() (TR0 = 1)
#define T0_close() (TR0 = 0)

int T0_open_method1(uint8_t GATE_, uint8_t CT_);
int T1_open_method1(uint8_t GATE_, uint8_t CT_);
uint16_t T0_get_counter();
int intr_open_int0(char IT0_, char PX0_);
int intr_open_int1(char IT1_, char PX1_);

#endif