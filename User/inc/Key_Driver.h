#ifndef __KEY_DRIVER_H__
#define __KEY_DRIVER_H__


#include "stm32f1xx.h"
#include "main.h"
#include "Run_task.h"
#include "Data_Structure.h"

 /** 按键按下标置宏
	* 按键按下为低电平，设置 KEY_ON = 0， KEY_OFF = 1
	* 若按键按下为高电平，把宏设置成KEY_ON = 1，KEY_OFF = 0 即可
	*/
#define KEY_ON	0
#define KEY_OFF	1

//引脚定义
/*******************************************************
**K1----PB15
**K2----PB14
**K3----PA0
**K4----PB12
*******************************************************/
#define KEY1_PIN                  GPIO_PIN_15  
#define KEY2_PIN                  GPIO_PIN_14     
#define KEY3_PIN                  GPIO_PIN_0      
#define KEY4_PIN                  GPIO_PIN_12      

#define KEY_124_GPIO_PORT			GPIOB                      
#define KEY_124_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define KEY_3_GPIO_PORT				GPIOA                      
#define KEY_3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()  
/*******************************************************/

#define KEY_DOWN		0
#define KEY_UP			1
#define	NO_KEY			0xff

#define IOKEY_NUM		4

//#define KEY_BASE_CNT  5
//#define KEY_LONG_CNT  (75)
//#define KEY_HOLD_CNT  15

#define KEY_BASE_CNT  10
#define KEY_LONG_CNT  (100)
#define KEY_HOLD_CNT  25

////////////////IO Key//////////////////////
#define IOKEY_SHORT		\
						MSG_IOKEY1_SHORT,\
						MSG_IOKEY2_SHORT,\
						MSG_IOKEY3_SHORT, \
						MSG_IOKEY4_SHORT,
							
							

//长按
#define IOKEY_LONG		\
						MSG_IOKEY1_LONG,\
						MSG_IOKEY2_LONG,\
						MSG_IOKEY3_LONG,\
						MSG_IOKEY4_LONG,



//连按
#define IOKEY_HOLD		\
						MSG_IOKEY1_HOLD,\
						MSG_IOKEY2_HOLD,\
						MSG_IOKEY3_HOLD,\
						MSG_IOKEY4_HOLD,

//长按抬起
#define IOKEY_LONG_UP	\
						MSG_IOKEY1_LONG_UP,\
						MSG_IOKEY2_LONG_UP,\
						MSG_IOKEY3_LONG_UP,\
						MSG_IOKEY4_LONG_UP

struct iokey_type{
	GPIO_TypeDef* GPIO;
	uint32_t pin;
	uint32_t keyval;
};


typedef enum en_key_type
{
	EN_KEY_TYPE_MENU = 0,
	EN_KEY_TYPE_UP,
	EN_KEY_TYPE_DOWN,
	EN_KEY_TYPE_LEFT,
	EN_KEY_TYPE_RIGHT,
	EN_KEY_TYPE_NONE,

} en_key_type_t, *pen_key_type_t;


void Key_Init(void);
void board_keyScan(void);

#endif /* ____KEY_DRIVER_H */



