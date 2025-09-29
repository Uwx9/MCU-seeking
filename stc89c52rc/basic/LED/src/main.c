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
	intr_open_int0(0, 1);	// 低电平有效, 高优先级
	P0_set0();
	while (1);
}

int a = 0;
void INT0_FUNC(void) interrupt 0 using 1	// INT0的中断源为0 使用寄存器组1保存快照
{
	delay_ms(25);
	if (a == 0) {
		P0_set1();
		a++;
	}
	else {
		P0_set0();
		a = 0;
	} 
}