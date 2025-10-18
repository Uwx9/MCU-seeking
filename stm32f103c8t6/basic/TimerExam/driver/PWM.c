/**
 * @file PWM.c
 * @author Unique9
 * @brief TIM3_CH1输出PWM波形, PA6
 * @version 0.1
 * @date 2025-10-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "stm32f10x.h"

/* 这里把TIM3用作pwm输出 */
void PWMTIM3_init()
{
	// 打开TIM3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// 时钟源选择内部时钟
	TIM_InternalClockConfig(TIM3);

	/* GPIO初始化, 这里TIM3比较单元1的输出在PA6 
	   输出要改为复用开漏/推挽输出, 即输出来源改为复用功能输出
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_initstructure;
	GPIO_initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_initstructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_initstructure);
	
	// 时基单元初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;		
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	// 仅TIM1/8用
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// 初始输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);							// 虽然有些字段用不上, 但是还是要赋初值, 毕竟是局部变量
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// 输出比较模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		// 有效电平为高电平
	TIM_OCInitStructure.TIM_Pulse = 0;								// 此即CCR
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);		// PA6

	
	// TIM3使能
	TIM_Cmd(TIM3, ENABLE);

}

// 更改CCR以改变占空比
void PWMTIM3_set_compare(uint16_t CCRx, uint16_t compare)
{
	if (CCRx == 1) {
		TIM_SetCompare1(TIM3, compare); 
	} else if (CCRx == 2) {
		TIM_SetCompare2(TIM3, compare); 
	}
}

// 单独更新TIMx的PSC
void PWNTIM3_set_psc(uint16_t psc)
{
	TIM_PrescalerConfig(TIM3, psc, TIM_PSCReloadMode_Update);
}
