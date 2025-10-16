#include "../user/stm32f10x_it.h"
#include "../system/delay.h"

void key_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_initstructure;
	GPIO_initstructure.GPIO_Mode = GPIO_Mode_IPU;	// 上拉输入
	GPIO_initstructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_initstructure);
}

/* 这里用的两个按钮分别是PB0和PB6,PB0控制led0, PB6控制led1,  */
int8_t get_keynum()
{
	int8_t retkey = -1;
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);
		Delay_ms(20);
		retkey = 0;
	}
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		Delay_ms(20);
		retkey = 1;
	}
	return retkey;
}