#include "stm32f10x.h"

extern uint32_t num;

void TIM2_Init()
{
	// 打开TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 时钟源选择内部时钟
	TIM_InternalClockConfig(TIM2);

	// 时基单元初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;		// 计数10k次溢出, 1s溢出一次 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;	// 72M / 72*10^2 = 10k频率
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	// 仅TIM1/8用
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 由于上面那句会产生更新事件导致直接进入中断, 所以这里清楚中断标志位
	TIM_ClearFlag(TIM2, TIM_IT_Update);

	// 允许TIM2中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// 设置NVIC优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// 初始化NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;		// 这个应该是中断请求通道号
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	// TIM2使能
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		num++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}