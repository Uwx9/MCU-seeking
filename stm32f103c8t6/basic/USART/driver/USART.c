#include "stm32f10x.h"
#include "../driver/OLED.h"
#include "stdarg.h"
#include "stdio.h"

uint8_t TXpkg[4];
char RXpkg[100];
uint8_t state = 0;
uint8_t rx_flag = 0;

/**
 * @brief USART1初始化
 * 
 */
void MyUSART_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// 这里的信号输出来源内部, 用复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	// 波特率9600, 无流控, 发送模式, 无校验, 1位停止位, 8位字长
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	// 接收中断配置
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 发送一个字节
 * 
 * @param data 
 */
void usart_send_byte(uint8_t data)
{
	USART_SendData(USART1, data);
	
	// 等待发送完当前data
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
 * @brief 发送一个char数组
 * 
 * @param arr 
 */
void usart_send_byte_arr(uint8_t* arr, uint16_t lenth)
{
	for (uint16_t i = 0; i < lenth; i++) {
		usart_send_byte(arr[i]);
	}
}

/**
 * @brief 发送一个字符串
 * 
 */
void usart_send_string(char* str)
{
	while (*str != '\0') {
		usart_send_byte(*str);
		str++;
	}
}


/**
 * @brief 拿到Num的第i位
 * 
 * @param Num 
 * @param Lenth 
 * @return uint8_t 
 */
uint8_t get_num_highest(uint32_t Num, uint8_t i)
{
	uint32_t div = 1;
	for (uint8_t j = 0; j < i - 1; j++) {
		div *= 10;	
	}
	uint8_t result = '0' + Num / div % 10; 
	return result;
}

/**
 * @brief 发送一个数字
 * 
 */
void usart_send_num(uint32_t num, uint8_t lenth)
{
	for (uint8_t i = lenth; i > 0; i--) {
		usart_send_byte(get_num_highest(num, i));
	}
}

/**
 * @brief printf重定向
 * 
 */
void usart_printf(char* format, ...)
{
	char SendString[100];
	va_list arg;
	va_start(arg, format);	// 拿到第一个参数的地址, 其实就是format在栈中的地址
	vsprintf(SendString, format, arg);	// 这里是用arg去挨个访问变参做替换, 将替换后的字符串放到SendString
	va_end(arg);	// 释放arg, 让其=NULL
	usart_send_string(SendString);
}

/* 接收相关函数 */
void USART1_IRQHandler(void)
{
	static uint8_t state = 0;
	static uint8_t RXptr = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t data = USART_ReceiveData(USART1);

		if (state == 0)	{
			if (data == '@' && rx_flag ==0) {	// 数据处理完了才接收新的数据
				state = 1;
				RXptr = 0;
			}
		} else if (state == 1) {
			if (data == '\r')
				state = 2;
			else {
				RXpkg[RXptr] = data;
				RXptr++;
			}
		} else if (state == 2) {
			if (data == '\n') {
				RXpkg[RXptr] = '\0';
				rx_flag = 1;
				state = 0;
			}
		}

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

uint8_t get_usart_flag()
{
	if (rx_flag == 1) {
		rx_flag = 0;
		return 1;
	}
	return 0;
}

/**
 * @brief 发送数据包, 包头ff, 包尾fe, 数据载荷4位
 * 
 */
void usart_send_datapkg()
{
	usart_send_byte(0xff);
	usart_send_byte_arr(TXpkg, 4);
	usart_send_byte(0xfe);
}