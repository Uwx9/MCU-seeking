#include "../user/stm32f10x_it.h"

/* led0接PA0口, led1接PA1口 */

/* 这里只初始化PA0和PA1 */
void led_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_initstructure;
	GPIO_initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initstructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_initstructure);
}

void led0_on()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void led0_off()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void led0_turn()
{
	uint8_t flag = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0);
	if (flag == 0)
		led0_off();
	else
		led0_on();
}

void led1_on()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void led1_off()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void led1_turn()
{
	uint8_t flag = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1);
	if (flag == 0)
		led1_off();
	else
		led1_on();
}
