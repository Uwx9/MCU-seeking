/**
 * @file IC.h
 * @author Unique9
 * @brief TIM2CH1(PA0)输入捕获, CCR1
 * @version 0.1
 * @date 2025-10-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "stm32f10x.h"

void IC_init()
{

	// 这里是把TIM2产生的pwm波作为输入, 由于捕获上升沿所以下拉输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_InternalClockConfig(TIM2);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		// 1M hz	
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	// 仅TIM1/8用
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	// 配置输入捕获单元1
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xf;	// 滤波器采样
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// 上升沿触发捕获
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// 不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	// 直连通道
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);		// 对另一个通道做相反配置, 这里是对通道2配置下降沿交叉通道

	/* 配置主从模式以自动让CCR置0 */
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);		// 触发源选择
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);		// 设置从模式

	TIM_Cmd(TIM2, ENABLE);
}

uint32_t IC_get_freq()
{
	return 1000000 / (TIM_GetCapture1(TIM2) + 1);
}

uint32_t IC_get_duty()
{
	return (uint32_t)((1.0 * TIM_GetCapture2(TIM2) / TIM_GetCapture1(TIM2)) * 100);		// 把100先乘了会把CCR2的误差放大, 所以先除 
}