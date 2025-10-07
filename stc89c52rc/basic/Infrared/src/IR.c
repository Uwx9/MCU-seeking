#include "REGX52.H"
#include "INTRINS.H"
#include "stdint.h"
#include "interrupt.h"
#include "delay.h"

uint8_t IR_status = 0;
uint16_t IR_fall2fall;
uint8_t Data[4] = {0};
uint8_t Data_ptr = 0;
uint8_t IR_address;
uint8_t IR_command;
uint8_t Data_flag = 0;
uint8_t Repeat_flag = 0;

void IR_init()
{
	T0_open_method1(0, 0);
	T0_close();
	intr_open_int0(1, 0);
}

void INT_FUNC_INT0() interrupt 0	// 按下一个按键应该会得到所有波形
{
	if (IR_status == 0) {
		T0_open();
		T0_set_initvalue(0);
		IR_status = 1;
	} else if (IR_status == 1) {	// 如果是开始信号, 下一次进中断应该是分析数据了
		IR_fall2fall = T0_get_counter();
		T0_set_initvalue(0);

		if (IR_fall2fall >= 12442 - 500 && IR_fall2fall <= 12442 + 500) {	// 起始信号
			IR_status = 2;
		} else if (IR_fall2fall >= 10368 - 500 && IR_fall2fall <= 10368 + 500) {	// 重发信号
			IR_status = 0;
			Repeat_flag = 1;
			T0_close();
		} else {
			IR_status = 1;
		}

	} else if (IR_status == 2) { // 分析数据
		IR_fall2fall = T0_get_counter();
		T0_set_initvalue(0);

		// 数据分析
		if (IR_fall2fall >= 1032 - 500 && IR_fall2fall <= 1032 + 500) { // 数据0
			Data[Data_ptr / 8] &= ~(0x01 << Data_ptr % 8);
			Data_ptr++;
		} else if (IR_fall2fall >= 2074 - 500 && IR_fall2fall <= 2074 + 500) {	// 数据1
			Data[Data_ptr / 8] |= (0x01 << Data_ptr % 8);
			Data_ptr++;
		} else {
			IR_status = 1;
			Data_ptr = 0;
		}

		// 满32位就判断正确性
		if (Data_ptr >= 32) {
			if ((Data[0] == ~Data[1]) && (Data[2] == ~Data[3])) {	// 数据正确
				Data_flag = 1;
				IR_address = Data[0];
				IR_command= Data[2];
			}

			Data_ptr = 0;
			IR_status = 0;
			T0_close();
		}

	}

}
