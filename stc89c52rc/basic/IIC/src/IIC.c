#include "REGX52.H"
#include "INTRINS.H"
#define nop() _nop_()

sbit SDA = P2^1;
sbit SCL = P2^0;
bit ack;	// ack为0表示无应答, 为1表示有应答

/* 以下Start 12C为开始信号发送函数,用于发送开始信号。
 * 该函数执行时,先让sCL, SDA都为高电平,
 * 然后在保持SCL为高电平时让SDA变为低电平,
 *  SDA由高电平变为低电平形成一个下降沿即为开始信号,
 * 接着SCL变为低电平，SDA 可以变化电平准备传送数据
 * */
static void IIC_start()
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
static void IIC_stop()
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
static void IIC_send_byte(unsigned char sdata)
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
	SCL = 0;		// 接收完ack, scl休息
	nop();
	nop();
}

/*以下ReceiveByte为字节数据接收函数,用于接收一个8位数据。
该函数执行时,从SDA线由高到低逐位读取8位数据并存放到变量rdat中,
在sCL为高电平时, SDA值被主器件读取,因为只接收8位数据,
故8位数据接收完后，主器件不发送 ACK 应答信号，如需发送 ACK 信号可使用应答函数*/
static unsigned char IIC_receive_byte()
{
	unsigned char rdata = 0;
	unsigned char bit_idx;
	SDA = 1;	// sda 为默认的高电平
	for (bit_idx = 0; bit_idx < 8; bit_idx++) {
		nop();
		SCL = 0;	// scl为低电平时才允许sda变化准备数据
		nop();		// 准备数据
		nop();
		nop();
		nop();
		nop();
		SCL = 1;
		nop();
		nop();
		rdata = rdata << 1;
		if (SDA == 1)
			rdata += 1;
		nop();		// 再维持scl高电平多一会
		nop();
	}
	SCL = 0;
	nop();
	nop();
	return rdata;
}

static void IIC_ACK()
{
	SDA = 0;
	nop();
	nop();
	nop();
	
	SCL = 1;
	nop();
	nop();
	nop();
	nop();
	nop();
	SCL = 0;
	nop();
	nop();
}

static void IIC_NOACK()
{
	SDA = 1;
	nop();
	nop();
	nop();
	
	SCL = 1;
	nop();
	nop();
	nop();
	nop();
	nop();
	SCL = 0;
	nop();
	nop();
}

/**
  * @brief  立即地址写操作是指不发送字节地址而是直接写上次操作地址N之后的地址N+1的数据
  * @param  从机地址,要发送的数据
  * @retval 成功返回0, 失败返回-1
  */
int IIC_write_byte_no_addr(unsigned char sladr, unsigned char sdata)
{
	IIC_start();
	IIC_send_byte(sladr);	// 发送7位从机地址和一位读写位
	if (ack == 0)	// 无应答
		return -1;
	IIC_send_byte(sdata);
	if (ack == 0)
		return -1;
	IIC_stop();
	return 0;	
}

/**
  * @brief  写字符串
  * @param  从机地址, 写入的地址, 发送字符串的地址, 字符个数
  * @retval 成功返回0, 失败返回-1
  */
int IIC_write_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* sendstr, unsigned char cnt)
{
	unsigned char i;
	IIC_start();
	IIC_send_byte(sladr);
	if (ack == 0)
		return -1;
	IIC_send_byte(subaddr);
	if (ack == 0)
		return -1;

	for (i = 0; i < cnt; i++) {
		IIC_send_byte(sendstr[i]);
		if (ack == 0) 
			return -1;
	}
	IIC_stop();
	return 0;
}

/**
  * @brief  立即地址读操作是指不发送字节地址而是直接读上次操作地址N之后的地址N+1的数据
  * @param  从机地址, 接收到的数据存放地址
  * @retval 成功返回0, 失败返回-1
  */
int IIC_read_byte(unsigned char sladr, unsigned char* rdata)
{
	IIC_start();
	IIC_send_byte(sladr | 0x01);
	if (ack == 0) 
		return -1;
	*rdata = IIC_receive_byte();
	IIC_NOACK();
	IIC_stop();
	return 0;
}


/**
  * @brief  连续读操作
  * @param  从机地址, 要读的地址, 读取的数据存放地址, 要读的数量
  * @retval 成功返回0, 失败返回-1
  */
int IIC_read_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* readed_str, unsigned char cnt)
{
	unsigned char i;
	IIC_start();
	IIC_send_byte(sladr);
	if (ack == 0)
		return -1;
	IIC_send_byte(subaddr);
	if (ack == 0)
		return -1;

	// 发送读信号准备读data
	IIC_start();
	IIC_send_byte(sladr | 0x01);
	if (ack == 0)
		return -1;
	for (i = 0; i < cnt - 1; i++){
		readed_str[i] = IIC_receive_byte();
		IIC_ACK();
	}
	readed_str[i] = IIC_receive_byte();
	IIC_NOACK();
	IIC_stop();
	return 0;
}