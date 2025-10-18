#include "stm32f10x.h"
#include "../driver/AD.h"
#include "../driver/OLED.h"

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