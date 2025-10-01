#include <reg52.h>
#include <stdint.h>
#include "timer.h"

void make_1kHz_square_wave()
{
	T0_open_method1(0, 0);	// 定时方式打开T0

	// 设置初值
	TH0 = (65536 - 500) / 256;
	TL0 = (65536 - 500) % 256;
	while (1);

}

void main(void)
{
	make_1kHz_square_wave();
}

void INT_T0_FUNC() interrupt 1 using 1
{
	// 重新设置初值
	TH0 = (65536 - 500) / 256;
	TL0 = (65536 - 500) % 256;

	P10 = !P10;
}