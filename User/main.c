/******************************欢迎大家进入 《芯知识学堂》 *********************************
**
**
**	
**	     ﹎    ●  ○ .﹎                        							★  作者：四叶草     
**	****┈ ┈ /█\/▓\ ﹎ ┈****                   						★单片机&工控技术QQ群： 82558344 ★        
**	 ☆°★    ∴°°∴ ☆°．·★°°Oooo          ★      		★物联网&嵌入式群QQ群: 544101253 ★
**	 ▅▆▇█████▇▆▅▃▂┈﹎▂▃▅▆    __▁▂▃▁__ ●     ★微信群向群主提交密语:  获取密钥★
										                    ^                                       |
																																								|
																														《好看的皮囊千篇一律，有料的大脑万一挑一
																																研发的梦想带你入门，无尽的学识助你成长》

本学堂走的是开源免费路线、里面所有的原创优质文章、书籍、视频就是免费获取！！！
                       独学而无友，孤陋而寡闻！
										      共同学习进步！！！
********************************************************************************************/

#include "include.h"
#include <stdio.h>
//#include "bmp.h"
#include "Run_task.h"


void Board_Init(void)
{
	HAL_Init();
	SystemClock_Config();    // 系统时钟初始化成72 MHz 
	NVIC_Configuration();    //中断组别的选择
	SysTick_Init();          //SYSTick的初始化
	Led_Init();              //初始化RGB灯
	USARTx_IintConfig();     //串口1的初始化
	printf("hello world!!\n");
	Key_Init();              //按键初始化
	Beep_Init();             //蜂鸣器初始化
	Sof_I2C_Init();
	power_ctl_register();
	board_power_ctl(PWR_OLED,PWR_ENABLE);
	board_power_ctl(PWR_INFRARED,PWR_ENABLE);
	OLED_Init();			 //初始化OLED显示屏
	Voltage_Init();          //电压采集初始化
//	SystemClock_Config();
	TIM_InitConfig();
	
//	OLED_DrawBMP(0,0,128,8,Peacock);
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	Board_Init();
	Run_Start();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef clkinitstruct = {0};
	RCC_OscInitTypeDef oscinitstruct = {0};
	
	HAL_RCC_DeInit();

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState        = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
	oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
	{
		/* Initialization Error */
		while(1); 
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	clocks dividers */
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
	{
	/* Initialization Error */
	while(1); 
	}
}


/************************ *****END OF FILE*****************************************/
