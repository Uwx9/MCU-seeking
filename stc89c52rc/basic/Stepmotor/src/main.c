#include <reg52.h>
#include "interrupt.h"
#include "delay.h"

int count = 0, percent = 0;


void main(void)
{
	// 开启INT1中断
	// intr_open_int1(1, 1);
	// intr_open_int0(1, 1);
	
	// 设置定时器初值为65526, 即10us来一次中断
	T0_open_method1(0, 0);
	TH0 = 0xff;
	TL0 = 0x9c;

	int speed = 0;
	while (1) {			
		if (P31 == 0) {
			while (P31 == 0);
			speed++;
			speed %= 4;
			if (speed == 0) percent = 0;
			else if (speed == 1) percent = 20;
			else if (speed == 2) percent = 50;
			else if (speed == 3) percent = 100;
		}
	}
}

void INT_T0_FUNC() interrupt 1
{
	// 定时器重新赋值
	TH0 = 0xff;
	TL0 = 0x9c;

	count++;
	count %= 100;
	if (count < percent) {
		P10 = 1;
	}
	else {
		P10 = 0;
	}

}


