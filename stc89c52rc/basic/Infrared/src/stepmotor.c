#include "REGX52.H"
#include "interrupt.h"
int count = 0, percent = 0;

/* 利用Timer1控制转速 */
void stepmotor_open()
{
// 设置定时器初值为65526, 即10us来一次中断
	T1_open_method1(0, 0);
	TH1 = 0xff;
	TL1 = 0x9c;
}

void INT_T1_FUNC() interrupt 3
{
	// 定时器重新赋值
	TH1 = 0xff;
	TL1 = 0x9c;

	count++;
	count %= 100;
	if (count < percent) {
		P1_3 = 1;
	}
	else {
		P1_3 = 0;
	}

}