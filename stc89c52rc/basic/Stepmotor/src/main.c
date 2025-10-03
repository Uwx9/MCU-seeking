#include <REGX52.H>
#include "key.h"
#include "interrupt.h"
#include "delay.h"
#include "nixie.h"

int count = 0, percent = 0;
unsigned char key_num = 0, speed = 0;


void main(void)
{
	// 开启INT1中断
	// intr_open_int1(1, 1);
	// intr_open_int0(1, 1);
	
	// 设置定时器初值为65526, 即10us来一次中断
	T0_open_method1(0, 0);
	TH0 = 0xff;
	TL0 = 0x9c;

	while (1) {			
		key_num = Key();
		if (key_num == 1) {
			speed++;
			speed %= 4;
			// if (speed == 0) percent = 0;
			// else if (speed == 1) percent = 20;
			// else if (speed == 2) percent = 50;
			// else if (speed == 3) percent = 100;
		}
		Nixie(1, speed);
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
		P1_0 = 1;
	}
	else {
		P1_0 = 0;
	}

}


