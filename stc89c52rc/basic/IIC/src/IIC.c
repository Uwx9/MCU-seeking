#include "REGX52.H"
#include "INTRINS.H"
#define nop() _nop_()

sbit SDA = P2^1;
sbit SCL = P2^0;
bit ack;

/* 以下Start 12C为开始信号发送函数,用于发送开始信号。
 * 该函数执行时,先让sCL, SDA都为高电平,
 * 然后在保持SCL为高电平时让SDA变为低电平,
 *  SDA由高电平变为低电平形成一个下降沿即为开始信号,
 * 接着SCL变为低电平，SDA 可以变化电平准备传送数据
 * */
void IIC_start()
{
	SDA = 1;
	nop();		// 延时1us让SDA顺利变为1
	SCL = 1;
	nop();		// 延时5us, 让sda, scl高电平持续时间>4us, 为了接收sda
	nop();
	nop();
	nop();
	nop();
	
	SDA = 0;	// sda 由高边低, 此为开始信号
	nop();		// 延时5us, 让sda开始信号持续时间>4us
	nop();
	nop();
	nop();
	nop();
	SCL = 0;	// 让scl为低电平, sda准备数据
	nop();		// 让scl高变低顺利进行
	nop();
}

/*以下 Stop2C为停止信号发送函数,用于发送停止信号。
该函数执行时,先让SDA为低电平,然后让SCL为高电平,再让SDA变为高电平。
在SCL为高电平时, SDA由低电平变为高电平形成一个上升沿即为停止信号*/ 
void IIC_stop()
{
	SDA = 0;	// 先变0, 准备从0变为1
	nop();
	SCL = 1;	// scl变为高电平并持续>4us, 以接收信号
	nop();
	nop();
	nop();
	nop();
	nop();

	SDA = 1;	// 0->1, 停止信号
	nop();
	nop();
	nop();
	nop();
}

/*以下SendByte为字节数据发送函数,用于发送一个8位数据。
该函数执行时,将变量sdat的8位数据(数据,或地址)由高到低逐位赋给SDA,
在SCL为高电平时, SDA值被接收器读取, 8位数据发送完后,让SDA=1,
在SCL为高电平时,若SDA电平被接收器拉低,表示有ACK信号应答;若SDA仍为高电平,表示无ACK信号应答或数据损坏*/
void IIC_send_byte(unsigned char sdata)
{
	unsigned char bit_idx;
	for (bit_idx = 0; bit_idx < 8; bit_idx++) {
		if ((sdata << bit_idx) & 0x80) {	// 从高位依次比较
			// 此为发送1的情况
			SDA = 1;
		} else {
			SDA = 0;
		}
		nop();
		
		SCL = 1;	// 准备接收数据
		nop();
		nop();
		nop();
		nop();
		nop();

		SCL = 0;	// 进下一次循环
	}
	// 此时8位数据发送完毕, 延时2us
	nop();
	nop();
	SDA = 1;		// 让sda变为默认的高电平, 等接收方ack拉低sda
	nop();
	nop();
	SCL = 1;		// 准备接收ack
	nop();
	nop();
	if (SDA == 1) 
		ack = 0;
	else 
		ack = 1;
	SCL = 0;
	nop();
	nop();
}

