/*********************************OWL-IOT32*********************************                                      
 
	                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)           
***************************************************************************/
#include "i2c_driver.h"



#ifdef DEVICE_I2C



static void GPIO_Pin_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

static void GPIO_Pin_Reset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

static uint8_t GPIO_Pin_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}




void i2c_delay_us(uint32_t nus)
{
		delay_us(nus);
}

Sof_i2c_TypeDef sof_oled_i2c = {
	.port = {
		.SDA_GPIOx = OLED_IIC_GPIO_PORT,
		.SCL_GPIOx = OLED_IIC_GPIO_PORT,
	},
	.sda = OLED_SDA_GPIO_PIN,
	.scl = OLED_SCLK_GPIO_PIN,
	.fops = {
		.gpio_set = GPIO_Pin_Set,
		.gpio_reset = GPIO_Pin_Reset,
		.gpio_read_bit = GPIO_Pin_Read,
		.delay_us = i2c_delay_us,
	},
};

i2c_device i2c_oled_dev1 = {
	.sof_i2c = &sof_oled_i2c,
	.slave_addr = 0x78,
};


void I2C_GPIOInitConfig(Sof_i2c_TypeDef* Sof_i2c_inode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC_APB2PeriphClockCmd(Sof_i2c_inode->port.SDA_Periph_Clock,ENABLE); 
	//RCC_APB2PeriphClockCmd(Sof_i2c_inode->port.SCL_Periph_CLOCK,ENABLE); 
	OLED_IIC_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Pin   = Sof_i2c_inode->scl;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_PULLUP;          //上拉模式
	HAL_GPIO_Init(Sof_i2c_inode->port.SCL_GPIOx, &GPIO_InitStructure);	//初始化GPIO
	
	GPIO_InitStructure.Pin 		= Sof_i2c_inode->sda;
	GPIO_InitStructure.Speed  = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull   = GPIO_PULLUP;          //上拉模式	
	HAL_GPIO_Init(Sof_i2c_inode->port.SDA_GPIOx, &GPIO_InitStructure);

	GPIO_Pin_Set(Sof_i2c_inode->port.SCL_GPIOx, Sof_i2c_inode->scl);
	GPIO_Pin_Set(Sof_i2c_inode->port.SDA_GPIOx, Sof_i2c_inode->sda);
}

static void SDA_OUT(Sof_i2c_TypeDef* Sof_i2c_inode)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.Pin   = Sof_i2c_inode->scl;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_PULLUP;          //上拉模式
	HAL_GPIO_Init(Sof_i2c_inode->port.SDA_GPIOx, &GPIO_InitStructure);	//初始化GPIO	

	Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
}

static void SDA_IN(Sof_i2c_TypeDef* Sof_i2c_inode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.Pin   = Sof_i2c_inode->scl;
	GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull  = GPIO_PULLUP;          //上拉模式
	HAL_GPIO_Init(Sof_i2c_inode->port.SDA_GPIOx, &GPIO_InitStructure);	//初始化GPIO	

	Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);	
}
/**************************************
**************************************/
void I2C_Start(Sof_i2c_TypeDef* Sof_i2c_inode)
{
		SDA_OUT(Sof_i2c_inode);
		Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
		Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
		delay_us(8);
		Sof_i2c_inode->fops.delay_us(8);
		Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
		delay_us(8);
		Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
}

/**************************************
**************************************/
void I2C_Stop(Sof_i2c_TypeDef* Sof_i2c_inode)
{
		SDA_OUT(Sof_i2c_inode);
		Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
		Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
		delay_us(8);
		Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
		delay_us(8);
}

/**************************************
**************************************/
void I2C_SendACK(Sof_i2c_TypeDef* Sof_i2c_inode,uint8_t ack)
{
	SDA_OUT(Sof_i2c_inode);
	Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
	delay_us(8);
	if(ack)
		Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
	else Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
	Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
	delay_us(8);
	Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
	delay_us(8);
}

/**************************************
**************************************/
uint8_t I2C_RecvACK(Sof_i2c_TypeDef* Sof_i2c_inode)
{
	uint8_t ucErrTime=0;
	uint8_t value = 0;

 	SDA_IN(Sof_i2c_inode);
	Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
	delay_us(4);	   
	Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
	delay_us(4);
	
	value = Sof_i2c_inode->fops.gpio_read_bit(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
	while(value)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop(Sof_i2c_inode);
			return 1;
		}
		value = Sof_i2c_inode->fops.gpio_read_bit(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
	}
	Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
	return 0;  
}

/**************************************
**************************************/
void I2C_SendByte(Sof_i2c_TypeDef* Sof_i2c_inode,uint8_t dat)
{
	uint8_t t;
	
	SDA_OUT(Sof_i2c_inode); 	    
    Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
    for(t=0;t<8;t++)
    {
			if(dat&0x80)
				Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
			else Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
					dat<<=1; 	  
			delay_us(5);
			Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
			delay_us(5); 
			Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
			delay_us(5);
    }
}

/**************************************
**************************************/	

uint8_t I2C_RecvByte(Sof_i2c_TypeDef* Sof_i2c_inode)
{
	int i = 0;
	uint8_t byte = 0;
	uint8_t value = 0;
	
	SDA_IN(Sof_i2c_inode);
	for(i = 0;i < 8;i++)
	{
		delay_us(5);
		Sof_i2c_inode->fops.gpio_set(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
		delay_us(5);
		byte <<= 1;
		value = Sof_i2c_inode->fops.gpio_read_bit(Sof_i2c_inode->port.SDA_GPIOx,Sof_i2c_inode->sda);
		if(value)
		{
			byte |= 0x01;
		}
		Sof_i2c_inode->fops.gpio_reset(Sof_i2c_inode->port.SCL_GPIOx,Sof_i2c_inode->scl);
		delay_us(5);
	}
	return byte;
}


/****************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
****************************************************************************/
uint8_t I2C_CheckDevice(Sof_i2c_TypeDef* Sof_i2c_inode,uint8_t _Address)
{
	uint8_t ucAck;

	I2C_Start(Sof_i2c_inode);		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	I2C_SendByte(Sof_i2c_inode,_Address | I2C_WR);
	ucAck = I2C_RecvACK(Sof_i2c_inode);	/* 检测设备的ACK应答 */

	I2C_Stop(Sof_i2c_inode);			/* 发送停止信号 */

	return ucAck;
}



void Sof_I2C_Init(void)
{
	I2C_GPIOInitConfig(&sof_oled_i2c);
}

/******************************************************************
*	函 数 名: eeprom_ReadBytes
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参：dev-------:I2C总线上的设备
*			 _usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
********************************************************************/
uint8_t I2C_dev_ReadBytes(i2c_device* dev,
							uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	I2C_Start(dev->sof_i2c);
	I2C_SendByte(dev->sof_i2c,dev->slave_addr|I2C_WR);
	
	/* 第3步：发送ACK */
	if (I2C_RecvACK(dev->sof_i2c) != 0)
	{
		goto cmd_fail;	
	}

	/* 第4步：发送字节地址 */
	I2C_SendByte(dev->sof_i2c,(uint8_t)_usAddress&0xff);
	
	/* 第5步：发送ACK */
	if (I2C_RecvACK(dev->sof_i2c) != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	I2C_Start(dev->sof_i2c);
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_SendByte(dev->sof_i2c,dev->slave_addr|I2C_RD);
	/* 第8步：发送ACK */
	if (I2C_RecvACK(dev->sof_i2c) != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}	
	
	/* 第9步：循环读取数据 */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = I2C_RecvByte(dev->sof_i2c);	/* 读1个字节 */
		
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _usSize - 1)
		{
			I2C_SendACK(dev->sof_i2c,0);	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			I2C_SendACK(dev->sof_i2c,1);	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	I2C_Stop(dev->sof_i2c);
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	I2C_Stop(dev->sof_i2c);
//	printf("Read fild!\r\n");
	return 0;
}


/**********************************************************************************
*	函 数 名: eeprom_WriteBytes
*	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
*	形    参：dev-------:I2C总线上的设备
*			 _usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pWriteBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
**********************************************************************************/
uint8_t I2C_dev_WriteBytes(i2c_device* dev,
							uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	

	usAddr = _usAddress;
	for (i = 0; i < _usSize; i++)
	{
		/* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
		if (i == 0)
		{
			/*　第０步：发停止信号，启动内部写操作　*/
			I2C_Stop(dev->sof_i2c);
			
			/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
				CLK频率为200KHz时，查询次数为30次左右
			*/
			for (m = 0; m < 100; m++)
			{				
				/* 第1步：发起I2C总线启动信号 */
				I2C_Start(dev->sof_i2c);
				
				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
				I2C_SendByte(dev->sof_i2c,dev->slave_addr|I2C_WR);
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (I2C_RecvACK(dev->sof_i2c) == 0)
				{
					break;
				}
			}
			/* 第4步：发送字节地址*/
			
			I2C_SendByte(dev->sof_i2c,(uint8_t)usAddr%256);
			
			/* 第5步：发送ACK */
			if (I2C_RecvACK(dev->sof_i2c) != 0)
			{
				goto cmd_fail;	/* 器件无应答 */
			}
		}
	
		/* 第6步：开始写入数据 */
		I2C_SendByte(dev->sof_i2c,_pWriteBuf[i]);
	
		/* 第7步：发送ACK */
		if (I2C_RecvACK(dev->sof_i2c) != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}

		usAddr++;	/* 地址增1 */		
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	I2C_Stop(dev->sof_i2c);
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	I2C_Stop(dev->sof_i2c);
//	printf("Write fild!\r\n");
	return 0;
}



#endif
