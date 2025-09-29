/**
 * CPU: 89C52
 * Freq: 12MHz
*/

#include <reg52.h>
#include <stdio.h>

void delay_100ms(void) //@12MHz
{
    unsigned char i, j;

    i = 195;
    j = 138;

    do
    {
        while (--j)
            ;
    } while (--i);
}

void delay(unsigned int t);
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
void P0_set1()
{
	P00 = P01 = P02 = P03 = P04 = P05 = P06 = P07 = 1;
}

void P0_set0()
{
	P00 = P01 = P02 = P03 = P04 = P05 = P06 = P07 = 0;
}

void main(void)
{
	int T = 0, Toff = 500;
	while (1) {
		for (T = 0; T < 500; T++) {
			P0_set1();
			delay(T);
			P0_set0();
			delay(500 - T);
		}

		for (Toff = 500; Toff > 0; Toff--) {
			P0_set1();
			delay(Toff);
			P0_set0();
			delay(500 - Toff);
		}

		delay_100ms();
	}
}

void delay(unsigned int t)
{
	while (t--);
}