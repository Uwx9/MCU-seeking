#include <reg52.h>
#include <stdint.h>

int T0_open_method1(uint8_t GATE_, uint8_t CT_)
{
	// 设置TMOD
	if (GATE_ != 0 && GATE_ != 1) return -1;
	if (CT_ != 0 && CT_ != 1) return -1;
	
	uint8_t TMOD_bit_2_3 = GATE_ << 3 | CT_ << 2;
	TMOD = TMOD_bit_2_3 | 1;

	// 设置TCON
	TR0 = 1;

	// 设置IE和IP, 即中断开关和优先级
	EA = 1;
	ET0 = 1;
	PT0 = 1;
	
	return 0;
}

int T1_open_method1(uint8_t GATE_, uint8_t CT_)
{
	// 设置TMOD
	if (GATE_ != 0 && GATE_ != 1) return -1;
	if (CT_ != 0 && CT_ != 1) return -1;
	
	uint8_t TMOD_bit_6_7 = GATE_ << 7 | CT_ << 6;
	TMOD = TMOD_bit_6_7 | 1;

	// 设置TCON
	TR1 = 1;

	// 设置IE和IP, 即中断开关和优先级
	EA = 1;
	ET1 = 1;
	PT1 = 1;
	
	return 0;
}