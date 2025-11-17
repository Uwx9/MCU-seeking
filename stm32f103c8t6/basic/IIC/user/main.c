#include "stm32f10x.h"
#include "../driver/OLED.h"
#include "../driver/MPU6050.h"

int16_t AX, AY, AZ, GX, GY, GZ;

int main(void)
{
	MPU6050_init();
	OLED_Init();

	while (1) {
		MPU6050_get_data(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
		OLED_ShowSignedNum(2, 1, AX, 5);					//OLED显示数据
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}