/**
 * CPU: 89C52
 * Freq: 12MHz
*/

#include <reg52.h>
#include <stdio.h>
#include <LED.h>

/* led从两边到中间依次亮
 *
// void main(void)
// {
//     while (1) {
// 		P00 = P07 = 1;
// 		delay_100ms();
// 		delay_100ms();
// 		P00 = P07 = 0;
		
// 		P01 = P06 = 1;
// 		delay_100ms();
// 		delay_100ms();
// 		P01 = P06 = 0;
		
// 		P02 = P05 = 1;
// 		delay_100ms();
// 		delay_100ms();
// 		P02 = P05 = 0;
		
// 		P03 = P04 = 1;
// 		delay_100ms();
// 		delay_100ms();
// 		P03 = P04 = 0;
// 	}
// }
 */

void main(void)
{
	LED_breath_by_P0();
}