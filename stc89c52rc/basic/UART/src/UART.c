#include "REGX52.H"

/**
  * @brief  串口初始化, 方式1
  * @param  无
  * @retval 无
  */
void UART_init()
{
	// 设置T1工作方式8位重载, 为串口提供波特率发生器
	PCON &= 0x7F;		// 波特率不倍速
	SCON = 0x50;		// 8位数据,可变波特率
	TMOD &= 0x0F;		// 设置定时器模式
	TMOD |= 0x20;		// 设置定时器模式
	TL1 = 0xFA;			// 设置定时初始值
	TH1 = 0xFA;			// 设置定时重载值, TL溢出后将TH的值放入TL
	ET1 = 0;			// 禁止定时器中断
	TR1 = 1;			// 定时器1开始计时
	EA = 1;				// 总开关
	ES = 1;				// 串口中断开关
}

void UART_send_byte(unsigned char Byte)
{
	SBUF = Byte;
	while (TI == 0);
	TI = 0;
}

void INT_FUNC_UART() interrupt 4
{
	if (RI == 1) {
		UART_send_byte(SBUF);
		P2 = SBUF;
		RI = 0;
	}
}