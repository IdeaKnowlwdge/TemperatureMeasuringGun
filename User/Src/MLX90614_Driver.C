#include "MLX90614_Driver.h"
#include "SysTick_Driver.h"
#include "stdint.h"
#include "i2c_driver.h"
#include "include.h"


#define ACK	 0 //应答
#define	NACK 1 //无应答
#define SA				0x00 //Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM存取命令
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM存取命令
#define RAM_TOBJ1		0x07 //To1 address in the eeprom 目标1温度,检测到的红外温度 -70.01 ~ 382.19度


/*******************************************************************************
* Function Name  : PEC_calculation
* Description    : Calculates the PEC of received bytes
* Input          : pec[]
* Output         : None
* Return         : pec[0]-this byte contains calculated crc value
*******************************************************************************/
uint8_t PEC_Calculation(uint8_t pec[])
{
    uint8_t 	crc[6];
    uint8_t	BitPosition=47;
    uint8_t	shift;
    uint8_t	i;
    uint8_t	j;
    uint8_t	temp;

    do
    {
        /*Load pattern value 0x000000000107*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}


i2c_device INFRARED_dev = {
	.sof_i2c = &sof_i2c2,
	.slave_addr = SA,
};

/******************************************************************
*	函 数 名: i2c_dev_ReadBytes
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参：dev-------:I2C总线上的设备
*			 _usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
********************************************************************/
uint8_t i2c_dev_ReadBytes(i2c_device* dev,
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
		delay_us(100);
	}
	
	/* 发送I2C总线停止信号 */
	I2C_Stop(dev->sof_i2c);
	return 0;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	I2C_Stop(dev->sof_i2c);
//	printf("Read fild!\r\n");
	return 1;
}



int infrared_read_data(i2c_device* dev,uint8_t checksum_array[],uint8_t* crc_checksum)
{
	int ret = 0;
	uint8_t ReadBuf[3] = {0};

	ret = i2c_dev_ReadBytes(dev,ReadBuf, RAM_ACCESS|RAM_TOBJ1,3);
	if(ret)
	{
		return -1;
	}

	checksum_array[5] = dev->slave_addr;		//device address and write bit
	checksum_array[4] = RAM_ACCESS|RAM_TOBJ1;	//
	checksum_array[3] = dev->slave_addr|1;	//device address and read bit
	checksum_array[2] = ReadBuf[0];			//Low data bit
	checksum_array[1] = ReadBuf[1];				//high data bit
	checksum_array[0] = 0;					//
	
	*crc_checksum = ReadBuf[2];
	
	return 0;
}

int get_infrared_data(uint16_t* data)
{
	int ret = 0;
	i2c_device* dev = &INFRARED_dev;
	uint8_t checksum_array[6] = {0};
	uint8_t crc_checksum = 0;
	uint8_t PecReg;				// PEC byte storage
    uint8_t DataL=0;			// Low data byte storage
    uint8_t DataH=0;			// High data byte storage

	ret = infrared_read_data(dev,checksum_array,&crc_checksum);
	if(ret)
	{
		return ret;
	}

	DataL = checksum_array[2];
	DataH = checksum_array[1];
	PecReg=PEC_Calculation(checksum_array);//Calculate CRC

	if(crc_checksum != PecReg)
	{
		return -1;
	}

	*data = (DataH<<8) | DataL;	//data=DataH:DataL
	
	return ret;
}

float infrared_ReadTemp(void)
{
	int status = 0;
	uint16_t data = 0;
	float temp;

	status = get_infrared_data(&data);
	if(status)
	{
		return 0;
	}

	temp = (float)data * 0.02 - 273.15;;

	return temp;
}

