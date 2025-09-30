/**
 * CPU: 89C52
 * Freq: 12MHz
*/

#include <reg52.h>
#include <stdio.h>
#include "LED.h"
#include "delay.h"
#include "interrupt.h"
#include "port.h"

void main(void)
{
	intr_open_int1(1, 1);
	while (1);
}

int a = 0;
void INT0_FUNC(void) interrupt 0 using 1	// INT0的中断源为0 使用寄存器组1保存快照
{
	Delay1ms(20);
	if (a == 0) {
		P0_set1();
		a++;
	}
	else {
		P0_set0();
		a = 0;
	} 
	Delay1ms(20);
}

void INT1_FUNC(void) interrupt 2 using 1	// INT0的中断源为2 使用寄存器组1保存快照
{
	if (!INT1) {
		Delay1ms(20);
		while (!INT1);
		Delay1ms(20);
		P0 = ~P0;
	}
}