#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "SysTick_Driver.h"
#include "i2c_driver.h"


#ifdef DEVICE_I2C
extern i2c_device i2c_oled_dev1;
#endif

//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PD6（SCL）
//              SDA   接PD7（SDA）            
 
/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{
#ifndef DEVICE_I2C
	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
#else
	
	I2C_Start(i2c_oled_dev1.sof_i2c);
#endif
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
#ifndef DEVICE_I2C	
	OLED_SCLK_Set() ;
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
#else
	I2C_Stop(i2c_oled_dev1.sof_i2c);
#endif	
}

void IIC_Wait_Ack(void)
{
#ifndef DEVICE_I2C	
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
#else

#endif	
}


/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{	
#ifndef 	DEVICE_I2C
		IIC_Start();
		Write_IIC_Byte(0x78);            //Slave address,SA0=0
		IIC_Wait_Ack();	
		Write_IIC_Byte(0x00);			//write command
		IIC_Wait_Ack();	
		Write_IIC_Byte(IIC_Command); 
		IIC_Wait_Ack();	
		IIC_Stop();
#else
		I2C_Start(i2c_oled_dev1.sof_i2c);
		I2C_SendByte(i2c_oled_dev1.sof_i2c, 0x78);
		I2C_RecvACK(i2c_oled_dev1.sof_i2c);
		I2C_SendByte(i2c_oled_dev1.sof_i2c, 0x00);
		I2C_RecvACK(i2c_oled_dev1.sof_i2c);
		I2C_SendByte(i2c_oled_dev1.sof_i2c, IIC_Command);
		I2C_RecvACK(i2c_oled_dev1.sof_i2c);
		I2C_Stop(i2c_oled_dev1.sof_i2c);
#endif		
}

/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
#ifndef 	DEVICE_I2C
	IIC_Start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
#else
	I2C_Start(i2c_oled_dev1.sof_i2c);
	I2C_SendByte(i2c_oled_dev1.sof_i2c, 0x78);
	I2C_RecvACK(i2c_oled_dev1.sof_i2c);
	I2C_SendByte(i2c_oled_dev1.sof_i2c, 0x40);
	I2C_RecvACK(i2c_oled_dev1.sof_i2c);
	I2C_SendByte(i2c_oled_dev1.sof_i2c, IIC_Data);
	I2C_RecvACK(i2c_oled_dev1.sof_i2c);
	I2C_Stop(i2c_oled_dev1.sof_i2c);
#endif		
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);		
	}
	else 
	{
		Write_IIC_Command(dat);	
	}


}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//坐标设置

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		
		for(n=0;n<128;n++)
			OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_DataClear(void)  
{  
	uint8_t i,n;		    
	for(i=2;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		
		for(n=0;n<128;n++)
			OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//==============================================================
//函数名：LED_ShowCHinese16x16(u8 x,u8 y,u8 num,u8 (*buf)[16]) num为第几个字，buf为中文数组（字体为16*16）
//功能描述：写入一组中文
//参数：显示的位置（x,y），y为页范围0～3，要显示的中文
//返回：无
//============================================================== 
void OLED_ShowCHinese16x16(uint8_t x,uint8_t y,uint8_t num,uint8_t (*buf)[16])
{      			    
	uint8_t i;
	OLED_Set_Pos(x,y);	
	for(i=0;i<16;i++)
	{
		OLED_WR_Byte(buf[2*num][i],OLED_DATA);
	}	
	OLED_Set_Pos(x,y+1);	

	for(i=0;i<16;i++)
	{	
		OLED_WR_Byte(buf[2*num+1][i],OLED_DATA);
	}			
		
}


/***********功能描述：显示显示BMP图片128×32起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOB_CLK_ENABLE();   //开启外设时钟
														   
    GPIO_InitStruct.Pin = OLED_SCLK_GPIO_PIN|OLED_SDA_GPIO_PIN; //选择要控制的GPIO引脚

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  //推完输出
    GPIO_InitStruct.Pull  = GPIO_PULLUP;          //上拉模式
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //设置引脚速率为高速 
    HAL_GPIO_Init(OLED_IIC_GPIO_PORT, &GPIO_InitStruct);	//初始化GPIO

		//空闲电平
		OLED_SCLK_Set();
		OLED_SDIN_Set();
	
	delay_ms(200);
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭显示
	
	OLED_WR_Byte(0x40,OLED_CMD);//---set low column address
	OLED_WR_Byte(0xB0,OLED_CMD);//---set high column address

	OLED_WR_Byte(0xC8,OLED_CMD);//-not offset

	OLED_WR_Byte(0x81,OLED_CMD);//设置对比度
	OLED_WR_Byte(0xff,OLED_CMD);

	OLED_WR_Byte(0xa1,OLED_CMD);//段重定向设置

	OLED_WR_Byte(0xa6,OLED_CMD);//
	
	OLED_WR_Byte(0xa8,OLED_CMD);//设置驱动路数
	OLED_WR_Byte(0x1f,OLED_CMD);
	
	OLED_WR_Byte(0xd3,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	
	OLED_WR_Byte(0xd5,OLED_CMD);
	OLED_WR_Byte(0xf0,OLED_CMD);
	
	OLED_WR_Byte(0xd9,OLED_CMD);
	OLED_WR_Byte(0x22,OLED_CMD);
	
	OLED_WR_Byte(0xda,OLED_CMD);
	OLED_WR_Byte(0x02,OLED_CMD);
	
	OLED_WR_Byte(0xdb,OLED_CMD);
	OLED_WR_Byte(0x49,OLED_CMD);
	
	OLED_WR_Byte(0x8d,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	
	OLED_WR_Byte(0xaf,OLED_CMD);
	OLED_Clear();
}  





























