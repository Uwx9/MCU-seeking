#include "REGX52.H"
#include "INTRINS.H"
#define nop() _nop_()

sbit EEPROM_SDA = P2^0;
sbit EEPROM_SCL = P2^1;
bit ack;	// ack为0表示无应答, 为1表示有应答
/* 以下Start 12C为开始信号发送函数,用于发送开始信号。
 * 该函数执行时,先让EEPROM_SCL, EEPROM_SDA都为高电平,
 * 然后在保持EEPROM_SCL为高电平时让EEPROM_SDA变为低电平,
 *  EEPROM_SDA由高电平变为低电平形成一个下降沿即为开始信号,
 * 接着EEPROM_SCL变为低电平，EEPROM_SDA 可以变化电平准备传送数据
 * */
static void IIC_start()
{
	EEPROM_SDA = 1;
	nop();		// 延时1us让EEPROM_SDA顺利变为1
	EEPROM_SCL = 1;
	nop();		// 延时5us, 让EEPROM_SDA, EEPROM_SCL高电平持续时间>4us, 为了接收EEPROM_SDA
	nop();
	nop();
	nop();
	nop();
	
	EEPROM_SDA = 0;	// EEPROM_SDA 由高边低, 此为开始信号
	nop();		// 延时5us, 让EEPROM_SDA开始信号持续时间>4us
	nop();
	nop();
	nop();
	nop();
	EEPROM_SCL = 0;	// 让EEPROM_SCL为低电平, EEPROM_SDA准备数据
	nop();		// 让EEPROM_SCL高变低顺利进行
	nop();
}

/*以下 Stop2C为停止信号发送函数,用于发送停止信号。
该函数执行时,先让EEPROM_SDA为低电平,然后让EEPROM_SCL为高电平,再让EEPROM_SDA变为高电平。
在EEPROM_SCL为高电平时, EEPROM_SDA由低电平变为高电平形成一个上升沿即为停止信号*/ 
static void IIC_stop()
{
	EEPROM_SDA = 0;	// 先变0, 准备从0变为1
	nop();
	EEPROM_SCL = 1;	// EEPROM_SCL变为高电平并持续>4us, 以接收信号
	nop();
	nop();
	nop();
	nop();
	nop();

	EEPROM_SDA = 1;	// 0->1, 停止信号
	nop();
	nop();
	nop();
	nop();
}

/*以下SendByte为字节数据发送函数,用于发送一个8位数据。
该函数执行时,将变量EEPROM_SDAt的8位数据(数据,或地址)由高到低逐位赋给EEPROM_SDA,
在EEPROM_SCL为高电平时, EEPROM_SDA值被接收器读取, 8位数据发送完后,让EEPROM_SDA=1,
在EEPROM_SCL为高电平时,若EEPROM_SDA电平被接收器拉低,表示有ACK信号应答;若EEPROM_SDA仍为高电平,表示无ACK信号应答或数据损坏*/
static void IIC_send_byte(unsigned char EEPROM_SDAta)
{
	unsigned char bit_idx;
	for (bit_idx = 0; bit_idx < 8; bit_idx++) {
		EEPROM_SDA = (EEPROM_SDAta & 0x80) ? 1 : 0;	// 从高位依次比较
		EEPROM_SDAta <<= 1;
		nop();
		
		EEPROM_SCL = 1;	// 准备接收数据
		nop();
		nop();
		nop();
		nop();
		nop();

		EEPROM_SCL = 0;	// 进下一次循环
	}
	// 此时8位数据发送完毕, 延时2us
	nop();
	nop();
	EEPROM_SDA = 1;		// 让EEPROM_SDA变为默认的高电平, 等接收方ack拉低EEPROM_SDA
	nop();
	nop();
	EEPROM_SCL = 1;		// 准备接收ack
	nop();
	nop();
	if (EEPROM_SDA == 1) 
		ack = 0;
	else 
		ack = 1;
	EEPROM_SCL = 0;		// 接收完ack, EEPROM_SCL休息
	nop();
	nop();
}

/*以下ReceiveByte为字节数据接收函数,用于接收一个8位数据。
该函数执行时,从EEPROM_SDA线由高到低逐位读取8位数据并存放到变量rdat中,
在EEPROM_SCL为高电平时, EEPROM_SDA值被主器件读取,因为只接收8位数据,
故8位数据接收完后，主器件不发送 ACK 应答信号，如需发送 ACK 信号可使用应答函数*/
static unsigned char IIC_receive_byte()
{
	unsigned char rdata = 0;
	unsigned char bit_idx;
	EEPROM_SDA = 1;	// EEPROM_SDA 为默认的高电平
	for (bit_idx = 0; bit_idx < 8; bit_idx++) {
		nop();
		EEPROM_SCL = 0;	// EEPROM_SCL为低电平时才允许EEPROM_SDA变化准备数据
		nop();		// 准备数据
		nop();
		nop();
		nop();
		nop();
		EEPROM_SCL = 1;
		nop();
		nop();
		rdata = rdata << 1;
		if (EEPROM_SDA == 1)
			rdata += 1;
		nop();		// 再维持EEPROM_SCL高电平多一会
		nop();
	}
	EEPROM_SCL = 0;
	nop();
	nop();
	return rdata;
}

static void IIC_ACK()
{
	EEPROM_SDA = 0;
	nop();
	nop();
	nop();
	
	EEPROM_SCL = 1;
	nop();
	nop();
	nop();
	nop();
	nop();
	EEPROM_SCL = 0;
	nop();
	nop();
}

static void IIC_NOACK()
{
	EEPROM_SDA = 1;
	nop();
	nop();
	nop();
	
	EEPROM_SCL = 1;
	nop();
	nop();
	nop();
	nop();
	nop();
	EEPROM_SCL = 0;
	nop();
	nop();
}

/**
  * @brief  立即地址写操作是指不发送字节地址而是直接写上次操作地址N之后的地址N+1的数据
  * @param  从机地址,要发送的数据
  * @retval 成功返回0, 失败返回-1
  */
int IIC_write_byte_no_addr(unsigned char sladr, unsigned char EEPROM_SDAta)
{
	IIC_start();
	IIC_send_byte(sladr);	// 发送7位从机地址和一位读写位
	if (ack == 0) {// 无应答
		IIC_stop();
		return -1;
	}
	IIC_send_byte(EEPROM_SDAta);
	if (ack == 0) {
		IIC_stop();
		return -1;
	}
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
	if (ack == 0) {
		IIC_stop();
		return -1;
	}
	IIC_send_byte(subaddr);
	if (ack == 0) {
		IIC_stop();
		return -1;
	}

	for (i = 0; i < cnt; i++) {
		IIC_send_byte(sendstr[i]);
		if (ack == 0) {
			IIC_stop();
			return -1;
		}
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
	if (ack == 0) {
		IIC_stop();
		return -1;
	}
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
	if (cnt < 1) 
		return -1;
	
	IIC_start();
	IIC_send_byte(sladr);
	if (ack == 0) {
		IIC_stop();
		return -1;
	}
	IIC_send_byte(subaddr);
	if (ack == 0) {
		IIC_stop();
		return -1;
	}

	// 发送读信号准备读data
	IIC_start();
	IIC_send_byte(sladr | 0x01);
	if (ack == 0) {
		IIC_stop();
		return -1;
	}
	for (i = 0; i < cnt - 1; i++){
		readed_str[i] = IIC_receive_byte();
		IIC_ACK();
	}
	readed_str[i] = IIC_receive_byte();
	IIC_NOACK();
	IIC_stop();
	return 0;
}