/**
 * @file AD.c
 * @author Unique9
 * @brief AD is so wonderful
 * @version 0.1
 * @date 2025-10-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "stm32f10x.h"

void AD_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	GPIO_InitTypeDef GPIO_initstructure;
	GPIO_initstructure.GPIO_Mode = GPIO_Mode_AIN;		// PA0口模拟输入
	GPIO_initstructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_initstructure);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		// 这句注释掉也能工作, 暂不清楚原因

	/* 配置规则通道 */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	

	ADC_InitTypeDef ADC_InitStructure;
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					// 单次转换
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					// 连续转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				// 12位数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 不用外部触发源
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					// 独立模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;								// 指定扫描模式下通道数
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						// 非扫描
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);

	/* 校准 */
	ADC_ResetCalibration(ADC1);						
	while (ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);						
	while (ADC_GetCalibrationStatus(ADC1));
	
	/* 连续转换时只需要触发一次, 就会一直不停的转换, 刷新数据寄存器的值, 直接读就行了 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);			// 触发转换
}

uint16_t AD_get_value()
{
	// ADC_SoftwareStartConvCmd(ADC1, ENABLE);			// 触发转换
	// while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); 	// 等待转换完成, 转换完成后会置1, 读取数据寄存器时会自动置0
	return ADC_GetConversionValue(ADC1);
}