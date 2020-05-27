#include "Key_Driver.h"
#include "SysTick_Driver.h"
#include "include.h"

/***********************************************
**	函数名 : Led_Init
**	功  能 : 初始化GPIOB口的12/13/14/15引脚为浮空输入	
**  参  数 : 无
**	返回值 :	 无	 
************************************************/
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启按键GPIOB口的时钟*/
	KEY_124_GPIO_CLK_ENABLE();
	KEY_3_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Pin = KEY1_PIN|KEY2_PIN|KEY4_PIN;  //选择按键的引脚
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;                  //设置引脚为输入模式
	GPIO_InitStructure.Pull = GPIO_PULLUP;       							//设置引脚不上拉也不下拉
	HAL_GPIO_Init(KEY_124_GPIO_PORT, &GPIO_InitStructure);     //使用上面的结构体初始化按键

	GPIO_InitStructure.Pin = KEY3_PIN; 
	HAL_GPIO_Init(KEY_3_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t Get_KeyStatus(struct iokey_type* port)
{
	return HAL_GPIO_ReadPin(port->GPIO, port->pin);
}

uint8_t iokey_value = 0x0f;

struct iokey_type iokey_check_table[] = {
	[0] = {
		.GPIO = KEY_124_GPIO_PORT,
		.pin = KEY1_PIN,
		.keyval = 0,
	},
	[1] = {
		.GPIO = KEY_124_GPIO_PORT,
		.pin = KEY2_PIN,
		.keyval = 0,
	},
	[2] = {
		.GPIO = KEY_3_GPIO_PORT,
		.pin = KEY3_PIN,
		.keyval = 0,
	},
	[3] = {
		.GPIO = KEY_124_GPIO_PORT,
		.pin = KEY4_PIN,
		.keyval = 0,
	},
};

uint8_t iokey_table[4][IOKEY_NUM] = {
	{IOKEY_SHORT},
	{IOKEY_LONG},
	{IOKEY_HOLD},
	{IOKEY_LONG_UP}
};


uint8_t io_keyscan(void)
{
	static uint8_t cnt;
	uint8_t value = 0;
	
	if(cnt == 0)
	{
		value = Get_KeyStatus(&iokey_check_table[cnt]);
		if(!value)
			iokey_value &= ~(1<<cnt);
		else 
			iokey_value |= (1<<cnt);
	}
	else if(cnt == 1)
	{
		value = Get_KeyStatus(&iokey_check_table[cnt]);
		if(!value)
			iokey_value &= ~(1<<cnt);
		else 
			iokey_value |= (1<<cnt);
	}
	else if(cnt == 2)
	{
		value = Get_KeyStatus(&iokey_check_table[cnt]);
		if(!value)
			iokey_value &= ~(1<<cnt);
		else 
			iokey_value |= (1<<cnt);
	}
	else if(cnt == 3)
	{
		value = Get_KeyStatus(&iokey_check_table[cnt]);
		if(!value)
			iokey_value &= ~(1<<cnt);
		else 
			iokey_value |= (1<<cnt);
	}
	else
	{
		cnt = 0;
	}
	
	cnt++;
	if(cnt >= 4)
		cnt = 0;
	
	return iokey_value;
}

uint8_t io_key(uint8_t key_value)
{
	uint8_t i = 0;

	if((key_value&0x0f) == 0x0f)
	{
		return NO_KEY;
	}
	
	key_value &= 0x0f;
	for(i = 0;i < IOKEY_NUM;i++)
	{
		if(!(key_value&(1<<i)))
		{
			break;
		}
	}
	
	return i;
}



uint8_t msg_key = NO_MSG;
uint8_t key_loop_counter = 0;
uint8_t key_type = NO_KEY;	//表示 K1  K2  K3  K4
uint8_t key_long_flag = 0;
void board_keyScan(void)
{
	uint8_t keyTemp = NO_KEY;
	uint8_t key_val = NO_KEY;	//表示短按 长按  连按 长按抬起
	
	keyTemp = io_key(io_keyscan());	//返回按下的按键在数组中的标号
	
	if(keyTemp == NO_KEY)
	{//Usart_SendString("X ");
		if((KEY_BASE_CNT < key_loop_counter)&&(key_loop_counter < KEY_LONG_CNT))
		{
			key_val = 0;
		}
		else if(KEY_LONG_CNT <= key_loop_counter)
		{Usart_SendString("W ");
			key_val = 3;
		}
		key_long_flag = 0;
		key_loop_counter = 0;
	}
	else if(keyTemp != NO_KEY)
	{
		key_loop_counter++;
		if(KEY_BASE_CNT == key_loop_counter)
		{
			key_long_flag = 0;
		}
		else if(KEY_LONG_CNT == key_loop_counter)
		{
			key_val = 1;
			key_long_flag = 1;
		}
		else if((KEY_LONG_CNT + KEY_HOLD_CNT) == key_loop_counter)
		{
			key_val = 2;
			key_loop_counter = KEY_LONG_CNT;
		}
	}
	
	if (keyTemp != key_type)
	{
		key_loop_counter = 0;
	}
	
	if((NO_KEY != key_type) && (key_val != NO_KEY))
	{
		msg_key = iokey_table[key_val][key_type];
		
		if (msg_key == NO_MSG)
			goto _exit_keyScan;
		Usart_SendString("H ");
//		Clear_msg_Fifo();
		put_msg_Fifo(msg_key);
		msg_key = NO_MSG;
//		iokey_value = 0x0f;
	}
	
_exit_keyScan:
	key_type = keyTemp;
}


