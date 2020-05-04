#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_

 
#include <inttypes.h>
#include "include.h"
#include "SysTick_Driver.h"


#define DEVICE_I2C

#ifdef DEVICE_I2C


#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

//描述一个端口的结构对象
struct Sof_i2c_Init
{
	uint32_t SDA_Periph_Clock;	//SDA RCC时钟
	uint32_t SCL_Periph_CLOCK;	//SCL RCC时钟
	GPIO_TypeDef* SDA_GPIOx;		//GPIO口,比如GPIOB 
	GPIO_TypeDef* SCL_GPIOx;		//GPIO口,比如GPIOB 
};

//描述软 I2c的操作函数集
struct sof_i2c_fops
{	
	void (*gpio_set)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	//设置GPIO输出高
	void (*gpio_reset)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	//设置GPIO输出低
	uint8_t (*gpio_read_bit)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//读取GPIO电平
	void (*delay_ms)(uint16_t nms);	//毫秒延时
	void (*delay_us)(uint32_t nus);	//微秒延时
};

typedef struct __arg_Sof_i2c_TypeDef
{
	struct Sof_i2c_Init port;	//描述端口的结构对象
	uint32_t sda;	//数据线
	uint32_t scl;	//时钟线
	uint32_t timeout;	//延时时间
	struct sof_i2c_fops fops;	//操作函数集
}Sof_i2c_TypeDef;


typedef struct __arg_i2c_device{
	Sof_i2c_TypeDef* sof_i2c;
	uint8_t slave_addr;
}i2c_device;


extern Sof_i2c_TypeDef sof_i2c1;


void I2C_GPIOInitConfig(Sof_i2c_TypeDef* Sof_i2c_inode);
void I2C_Start(Sof_i2c_TypeDef* Sof_i2c_inode);
void I2C_Stop(Sof_i2c_TypeDef* Sof_i2c_inode);
void I2C_SendACK(Sof_i2c_TypeDef* Sof_i2c_inode,uint8_t ack);
uint8_t I2C_RecvACK(Sof_i2c_TypeDef* Sof_i2c_inode);
void I2C_SendByte(Sof_i2c_TypeDef* Sof_i2c_inode,uint8_t dat);
uint8_t I2C_RecvByte(Sof_i2c_TypeDef* Sof_i2c_inode);
uint8_t I2C_CheckDevice(Sof_i2c_TypeDef* Sof_i2c_inode,uint8_t _Address);
void Sof_I2C_Init(void);


uint8_t I2C_dev_ReadBytes(i2c_device* dev,
							uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t I2C_dev_WriteBytes(i2c_device* dev,
							uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);

#endif

#endif
