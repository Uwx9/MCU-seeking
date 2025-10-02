#include <stdint.h>
#define _nop_() __asm nop __endasm

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

void Delay1ms(uint32_t ms)	//@11.0592MHz
{
	while (ms--) {
		unsigned char i, j;

		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}

void delay(unsigned int t)
{
	while (t--);
}

