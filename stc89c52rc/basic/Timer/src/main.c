#include <reg52.h>
#include <stdint.h>
#include "timer.h"

void make_1khz_square_wave();
void make_1hz_square_wave();

void main(void)
{
	make_1hz_square_wave();
}

void make_1khz_square_wave()
{
	T0_open_method1(0, 0);	// 定时方式打开T0

	// 设置初值
	TH0 = (65536 - 500) / 256;
	TL0 = (65536 - 500) % 256;
	while (1);

}

void make_1hz_square_wave()
{
	T1_open_method1(0, 0);	// 定时方式打开T1

	// 设置初值, 这里是50ms发送一次中断, 要得到1hz的方波就要10次中断改变电平
	TH0 = (65536 - 50000) / 256;
	TL0 = (65536 - 50000) % 256;
	while (1);

}

void INT_T0_FUNC() interrupt 1 using 1
{
	// 重新设置初值
	TH0 = (65536 - 500) / 256;
	TL0 = (65536 - 500) % 256;

	P10 = !P10;
}

uint32_t i = 0;
void INT_T1_FUNC() interrupt 3 using 1
{
	// 重新设置初值
	TH0 = (65536 - 50000) / 256;
	TL0 = (65536 - 50000) % 256;
	
	i++;
	if (i == 10) {
		i = 0;
		P10 = !P10;
	}
}
