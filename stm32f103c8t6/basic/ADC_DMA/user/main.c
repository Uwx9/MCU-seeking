#include "stm32f10x.h"
#include "../driver/AD.h"
#include "../driver/OLED.h"
#include "../driver/DMA.h"
#include "../system/delay.h"

/**
 * @brief ad模数转换
 * 
 */
uint16_t ad_value;
float volatge;
int main(void)
{
	OLED_Init();
	AD_init();
	OLED_ShowString(1, 1, "AD_Value:");
	OLED_ShowString(2, 1, "Volatge:");
	while (1) {
		ad_value = AD_get_value();
		OLED_ShowNum(1, 10, ad_value, 4);
		volatge = ad_value * 1.0 / 4095 * 3.3; 
		OLED_ShowNum(2, 9, volatge, 1);
		OLED_ShowNum(2, 11, (uint32_t)(volatge * 100) % 100, 2);
		OLED_ShowString(2, 10, ".");
		OLED_ShowString(2, 13, "V");
	}
}

// /**
//  * @brief DMA 数据转运
//  * 
//  */


// uint8_t DataB[] = {0, 0, 0, 0};

// int main()
// {
// 	uint8_t DataA[] = {1, 2, 3, 4};
// 	OLED_Init();
	
// 	OLED_ShowString(1, 1, "DataA");
// 	OLED_ShowString(3, 1, "DataB");
// 	OLED_ShowHexNum(1, 8, (uint32_t)DataA, 8);
// 	OLED_ShowHexNum(3, 8, (uint32_t)DataB, 8);
	
// 	myDMA_init((uint32_t)DataA, (uint32_t)DataB, 4);

// 	while (1)
// 	{
// 		DataA[0] ++;
// 		DataA[1] ++;
// 		DataA[2] ++;
// 		DataA[3] ++;
		
// 		OLED_ShowHexNum(2, 1, DataA[0], 2);
// 		OLED_ShowHexNum(2, 4, DataA[1], 2);
// 		OLED_ShowHexNum(2, 7, DataA[2], 2);
// 		OLED_ShowHexNum(2, 10, DataA[3], 2);
// 		OLED_ShowHexNum(4, 1, DataB[0], 2);
// 		OLED_ShowHexNum(4, 4, DataB[1], 2);
// 		OLED_ShowHexNum(4, 7, DataB[2], 2);
// 		OLED_ShowHexNum(4, 10, DataB[3], 2);
		
// 		Delay_ms(1000);
		
// 		myDMA_transfer();
		
// 		OLED_ShowHexNum(2, 1, DataA[0], 2);
// 		OLED_ShowHexNum(2, 4, DataA[1], 2);
// 		OLED_ShowHexNum(2, 7, DataA[2], 2);
// 		OLED_ShowHexNum(2, 10, DataA[3], 2);
// 		OLED_ShowHexNum(4, 1, DataB[0], 2);
// 		OLED_ShowHexNum(4, 4, DataB[1], 2);
// 		OLED_ShowHexNum(4, 7, DataB[2], 2);
// 		OLED_ShowHexNum(4, 10, DataB[3], 2);

// 		Delay_ms(1000);
		
// 	}
// }