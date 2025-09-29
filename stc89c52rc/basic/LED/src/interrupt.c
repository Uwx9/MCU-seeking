#include <reg52.h>

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
