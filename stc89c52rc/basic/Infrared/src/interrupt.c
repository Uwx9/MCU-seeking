#include <REGX52.H>
#include <stdint.h>

/****************** 时钟中断 *******************/

/**
 * @brief GATE一般为0, CT为0定时, CT为1计数
 */
int T0_open_method1(uint8_t GATE_, uint8_t CT_)
{
	// 设置TMOD
	if (GATE_ != 0 && GATE_ != 1) return -1;
	if (CT_ != 0 && CT_ != 1) return -1;
	
	TMOD &= 0xf0;
	TMOD |= (GATE_ << 3 | CT_ << 2) | 0x01;

	// 设置TCON
	TR0 = 1;
	TF0 = 0;

	// 设置IE和IP, 即中断开关和优先级
	EA = 1;
	ET0 = 1;
	PT0 = 0;
	return 0;
}

uint16_t T0_get_counter()
{
	return (TH0 << 8 | TL0);
}

int T1_open_method1(uint8_t GATE_, uint8_t CT_)
{
	// 设置TMOD
	if (GATE_ != 0 && GATE_ != 1) return -1;
	if (CT_ != 0 && CT_ != 1) return -1;
	
	TMOD &= 0x0f;
	TMOD |=  (GATE_ << 7 | CT_ << 6) | 0x10;

	// 设置TCON
	TR1 = 1;
	TF1 = 0;

	// 设置IE和IP, 即中断开关和优先级
	EA = 1;
	ET1 = 1;
	PT1 = 1;
	return 0;
}

/******************** 外部中断 ********************/

/* 打开中断INT0, 成功返回0, 失败返回-1 */
int intr_open_int0(char IT0_, char PX0_)
{
	if (IT0_ != 0 && IT0_ != 1) return -1;
	if (PX0_ != 0 && PX0_ != 1) return -1;
	IT0 = IT0_;	// 选择低电平或下降沿输入有效
	EA = 1;		// 开中断总闸
	EX0 = 1;	// 允许INT0中断
	PX0 = PX0_;	// 高优先级或低优先级
	return 0;
}

/* 打开中断INT1, 成功返回0, 失败返回-1 */
int intr_open_int1(char IT1_, char PX1_)
{
	if (IT1_ != 0 && IT1_ != 1) return -1;
	if (PX1_ != 0 && PX1_ != 1) return -1;
	IT1 = IT1_;	// 选择低电平或下降沿输入有效
	EA = 1;		// 开中断总闸
	EX1 = 1;	// 允许INT1中断
	PX1 = PX1_;	// 高优先级或低优先级
	P0 = 0;
	return 0;
}