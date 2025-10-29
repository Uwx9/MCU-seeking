#include "stm32f10x.h"

uint16_t DMA_size;

void myDMA_init(uint32_t addr_a, uint32_t addr_b, uint16_t size)
{
	DMA_size = size;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* 外设站点(只是名字叫这样, 没有什么特殊含义) */
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = size;	// 传输次数
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;		// 从外设寄存器传到存储	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;	// 软件触发
	DMA_InitStructure.DMA_MemoryBaseAddr = addr_b;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		//不重装
	DMA_InitStructure.DMA_PeripheralBaseAddr = addr_a;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// 外设站点数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;	// 传输完后地址自增
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);	// 对通道1用上面的参数初始化

	// DMA_Cmd(DMA1_Channel1, ENABLE);
}

void myDMA_transfer()
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	DMA_Cmd(DMA1_Channel1, ENABLE);

	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	// 等待转运完成
	DMA_ClearFlag(DMA1_FLAG_TC1);
}