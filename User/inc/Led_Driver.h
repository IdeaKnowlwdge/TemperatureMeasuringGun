#ifndef __LED_DRIVER_H_
#define __LED_DRIVER_H_


#include "stm32f1xx.h"
#include "main.h"


#define OFF		1
#define ON		0


//引脚定义
/*******************************************************/              	 
#define RGBLED_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define RGBLED_GPIO_PORT            GPIOA   

#define REDLED_GPIO_PIN  GPIO_PIN_8
#define GREENLED_GPIO_PIN  GPIO_PIN_11
#define BLUELED_GPIO_PIN GPIO_PIN_12
/*******************************************************/


//RGB灯对应引脚置高，置低电平的宏定义
/*******************************************************/     
#define RED_LED(a)	if(a) HAL_GPIO_WritePin(RGBLED_GPIO_PORT, REDLED_GPIO_PIN,GPIO_PIN_SET);\
										else HAL_GPIO_WritePin(RGBLED_GPIO_PORT, REDLED_GPIO_PIN,GPIO_PIN_RESET)
							
#define GREEN_LED(a)	if(a) HAL_GPIO_WritePin(RGBLED_GPIO_PORT, GREENLED_GPIO_PIN,GPIO_PIN_SET);\
										else HAL_GPIO_WritePin(RGBLED_GPIO_PORT, GREENLED_GPIO_PIN,GPIO_PIN_RESET)
							
#define BLUE_LED(a)	if(a) HAL_GPIO_WritePin(RGBLED_GPIO_PORT, BLUELED_GPIO_PIN,GPIO_PIN_SET);\
										else HAL_GPIO_WritePin(RGBLED_GPIO_PORT, BLUELED_GPIO_PIN,GPIO_PIN_RESET)

/*******************************************************/

//RGB灯类型的选择
/*******************************************************/ 						
typedef enum{

	Red = 0U,
	Green,
	Blue,

}LED_Type;
/*******************************************************/ 
		
//函数的声明
 void Led_Init(void);            
 void Led_Ctl(LED_Type Led_Type, uint8_t Led_State);  
 void  All_LedOn(void);
 void  All_LedOff(void);

#endif






