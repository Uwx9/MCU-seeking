#include "REGX52.H"
#include "INTRINS.H"
#include "stdint.h"

#define nop() _nop_()
#define IIC_TARGET_OLED
#define IFNOACK() do { if (ack == 0) {IIC_stop(); return -1;} } while(0)
// #define IIC_TARGET_EEPROM

#if defined (IIC_TARGET_EEPROM)
	sbit EEPROM_SDA = P2^0;
	sbit EEPROM_SCL = P2^1;
	bit ack;	// ack为0表示无应答, 为1表示有应答
	#define SDA EEPROM_SDA
	#define SCL EEPROM_SCL
#elif defined (IIC_TARGET_OLED)
	sbit OLED_SDA = P2^2;
	sbit OLED_SCL = P2^3;
	bit ack;	// ack为0表示无应答, 为1表示有应答
	#define SDA OLED_SDA
	#define SCL OLED_SCL
#endif

/* 以下Start 12C为开始信号发送函数,用于发送开始信号。
 * 该函数执行时,先让SCL, SDA都为高电平,
 * 然后在保持SCL为高电平时让SDA变为低电平,
 *  SDA由高电平变为低电平形成一个下降沿即为开始信号,
 * 接着SCL变为低电平，SDA 可以变化电平准备传送数据
 * */
static void IIC_start()
{
	SDA = 1;
	nop();		// 延时1us让SDA顺利变为1
	SCL = 1;
	nop();		// 延时5us, 让SDA, SCL高电平持续时间>4us, 为了接收SDA
	nop();
	nop();
	nop();
	nop();
	
	SDA = 0;	// SDA 由高边低, 此为开始信号
	nop();		// 延时5us, 让SDA开始信号持续时间>4us
	nop();
	nop();
	nop();
	nop();
	SCL = 0;	// 让SCL为低电平, SDA准备数据
	nop();		// 让SCL高变低顺利进行
	nop();
}

/*以下 Stop2C为停止信号发送函数,用于发送停止信号。
该函数执行时,先让SDA为低电平,然后让SCL为高电平,再让SDA变为高电平。
在SCL为高电平时, SDA由低电平变为高电平形成一个上升沿即为停止信号*/ 
static void IIC_stop()
{
	SDA = 0;	// 先变0, 准备从0变为1
	nop();
	SCL = 1;	// SCL变为高电平并持续>4us, 以接收信号
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
该函数执行时,将变量SDAt的8位数据(数据,或地址)由高到低逐位赋给SDA,
在SCL为高电平时, SDA值被接收器读取, 8位数据发送完后,让SDA=1,
在SCL为高电平时,若SDA电平被接收器拉低,表示有ACK信号应答;若SDA仍为高电平,表示无ACK信号应答或数据损坏*/
static void IIC_send_byte(unsigned char SDAta)
{
	unsigned char bit_idx;
	for (bit_idx = 0; bit_idx < 8; bit_idx++) {
		SDA = (SDAta & 0x80) ? 1 : 0;	// 从高位依次比较
		SDAta <<= 1;
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
	SDA = 1;		// 让SDA变为默认的高电平, 等接收方ack拉低SDA
	nop();
	nop();
	SCL = 1;		// 准备接收ack
	nop();
	nop();
	if (SDA == 1) 
		ack = 0;
	else 
		ack = 1;
	SCL = 0;		// 接收完ack, SCL休息
	nop();
	nop();
}

/*以下ReceiveByte为字节数据接收函数,用于接收一个8位数据。
该函数执行时,从SDA线由高到低逐位读取8位数据并存放到变量rdat中,
在SCL为高电平时, SDA值被主器件读取,因为只接收8位数据,
故8位数据接收完后，主器件不发送 ACK 应答信号，如需发送 ACK 信号可使用应答函数*/
static unsigned char IIC_receive_byte()
{
	unsigned char rdata = 0;
	unsigned char bit_idx;
	SDA = 1;	// SDA 为默认的高电平
	for (bit_idx = 0; bit_idx < 8; bit_idx++) {
		nop();
		SCL = 0;	// SCL为低电平时才允许SDA变化准备数据
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
		nop();		// 再维持SCL高电平多一会
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

// 真不能用数组, ram太少了, 就IIC挂载一个设备好了
uint8_t IIC_dev_addr = 0;
/**
 * @brief  IIC设备扫描函数
 */
void IIC_dev_scan()
{
	uint8_t sladdr;
	for (sladdr = 1; sladdr < 128; sladdr++) {
		IIC_start();
		IIC_send_byte(sladdr << 1);	// 给sladdr这里的设备发送写命令
		if (ack == 0)
			continue;
		else 
			IIC_dev_addr = sladdr;
	}
	IIC_stop();
	nop();nop();nop();
}


/**
  * @brief  立即地址写操作是指不发送字节地址而是直接写上次操作地址N之后的地址N+1的数据
  * @param  从机地址,要发送的数据
  * @retval 成功返回0, 失败返回-1
  */
int EEPROM_write_byte_no_addr(unsigned char sladr, unsigned char Sdata)
{
	IIC_start();
	IIC_send_byte(sladr);	// 发送7位从机地址和一位读写位
	if (ack == 0) {// 无应答
		IIC_stop();
		return -1;
	}
	IIC_send_byte(Sdata);
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
int EEPROM_write_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* sendstr, unsigned char cnt)
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
int EEPROM_read_byte(unsigned char sladr, unsigned char* rdata)
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
int EEPROM_read_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* readed_str, unsigned char cnt)
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


/****************** OLED驱动 ******************/

/**
 * @brief OLED写命令
 */
static int OLED_write_command(uint8_t command_)
{
	IIC_start();
	IIC_send_byte(0x3c << 1);	// 呼叫oled
	IFNOACK();
	IIC_send_byte(0x00);		// 发送control byte表示接下来我要发一个命令字节
	IFNOACK();
	IIC_send_byte(command_);		// 发送command
	IFNOACK();
	IIC_stop();
	return 0;
}

/**
 * @brief OLED写数据
 */
static int OLED_write_data(uint8_t data_)
{
	IIC_start();
	IIC_send_byte(0x3c << 1);	// 呼叫oled
	IFNOACK();
	IIC_send_byte(0x40);		// 发送control byte表示接下来我要发一个数据字节
	IFNOACK();
	IIC_send_byte(data_);		// 发送data
	IFNOACK();
	IIC_stop();
	return 0;
}

/**
 * @brief OLED_init
 */
void OLED_init()
{
    OLED_write_command(0xAE); // 关闭显示

    OLED_write_command(0xD5); // 设置显示时钟分频/振荡频率
    OLED_write_command(0x80); // 建议值

    OLED_write_command(0xA8); // 设置多路复用比
    OLED_write_command(0x3F); // 1/64

    OLED_write_command(0xD3); // 设置显示偏移
    OLED_write_command(0x00); // 无偏移

    OLED_write_command(0x40); // 起始行=0

    OLED_write_command(0x8D); // 充电泵设置
    OLED_write_command(0x14); // 使能充电泵

    OLED_write_command(0x20); // 寻址模式设置
    OLED_write_command(0x02); // 页地址模式

    OLED_write_command(0xA1); // 左右不颠倒, a0左右颠倒
    OLED_write_command(0xC8); // 上下不颠倒, c0上下颠倒

    OLED_write_command(0xDA); // COM 引脚硬件配置
    OLED_write_command(0x12); // 默认值

    OLED_write_command(0x81); // 对比度设置
    OLED_write_command(0xCF); // 对比度值

    OLED_write_command(0xD9); // 预充电周期
    OLED_write_command(0xF1); // 设置值

    OLED_write_command(0xDB); // VCOMH 电压
    OLED_write_command(0x40); // 默认 0.77*Vcc

    OLED_write_command(0xA4); // 恢复到 RAM 显示
    OLED_write_command(0xA6); // 正常显示（非反相）
    OLED_write_command(0xAF); // 开启显示
}

/**
 * @brief 设置坐标, 即哪一页和哪一行
 */
static void OLED_set_cursor(uint8_t page, uint8_t column)
{
	if (page > 7 || column > 127) return;
	OLED_write_command(0xb0 | page);			// 设置页号
	OLED_write_command(0x10 | column >> 4);		// 设置行高4位
	OLED_write_command(0x00 | (column & 0x0f));	// 设置行低4位
}

/**
 * @brief 清屏
 */
void OLED_clear()
{
	uint8_t page, column;
	for (page = 0; page < 8; page++) {
		OLED_set_cursor(page, 0);
		for (column = 0; column < 128; column++) {
			OLED_write_data(0x00);
		}
	}
}

/**
 * @brief 点亮一个字节
 * @param 页号0~7
 * @param 行号0~127
 */
void OLED_light_one_byte(uint8_t page, uint8_t column)
{
	if (page > 7 || column > 127) return;
	OLED_set_cursor(page, column);
	OLED_write_data(0xff);
}

/**
 * @brief 显示一个16x16字符, 高度占两页宽度16列, 屏上可显示4(行)x8(列)个字符
 * @param row4行中的一行0~3
 * @param column8列中的一列0~7
 */
void OLED_show_char_16x16(uint8_t char_row, uint8_t char_column, unsigned char readychar[][16])
{
	uint8_t i, j;
	uint8_t page = char_row, column = char_column;
	if (char_row > 3 || char_column > 7) return;
	
	for (i = 0; i < 2; i++) {
		OLED_set_cursor(page * 2 + i, column * 16);
		for (j = 0; j < 16; j++) {
			OLED_write_data(readychar[i][j]);
		}
	}

}

/**
 * @brief 显示一个 像素8x16(列x行) ascill码, 高度占2页宽度8列, 屏上可显示4(行)x16(列)个字符
 * @param row4行中的一行0~7
 * @param column8列中的一列0~15
 */
void OLED_show_char_8x16(uint8_t char_row, uint8_t char_column, unsigned char ascill, const unsigned char code*  readychar)
{
	uint8_t ascill_id = (uint8_t)ascill - 'a';
	uint8_t i, j;
	if (char_row > 3 || char_column > 15) return;
	
	for (i = 0; i < 2; i++) {
		OLED_set_cursor(char_row * 2 + i, char_column * 8);
		for (j = 0; j < 8; j++) {
			OLED_write_data(readychar[(ascill_id * 16) + (i * 8 + j)]);
		}
	}

}
/**
 * @brief 显示图片
 * @param 
 */
void OLED_show_image()
{

}
