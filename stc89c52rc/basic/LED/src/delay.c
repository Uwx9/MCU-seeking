#include <stdint.h>
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

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    
    for(i = 0; i < ms; i++) {
        // 外层：毫秒循环
        j = 920;  // 内层循环次数
        while(j--) {
            // 空循环体，依靠循环开销延时
        }
    }
}
void delay(unsigned int t)
{
	while (t--);
}

