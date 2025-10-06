#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_
#include <stdint.h>

int T0_open_method1(uint8_t GATE_, uint8_t CT_);
int T1_open_method1(uint8_t GATE_, uint8_t CT_);
int intr_open_int0(char IT0_, char PX0_);
int intr_open_int1(char IT1_, char PX1_);

#endif