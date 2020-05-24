#include "TIM_Driver.h"



TIM_HandleTypeDef    htim2;


/***********************************************************************
**函数功能：定时器2初始化配置，产生1s中断一次的计时
**计	算：计数器的时钟频率CK_CNT = f(CK_PSC)/(PCS[15:0]+1)
**			计数器的计时频率CK_CNT = 72MHz/(7199+1) = 10KHz
**			单次计数时间 	T(CNT) = 1/CK_CNT = 100us
**			定时器溢出时间	Tout = ((CNT[15:0] + 1)*[PSC[15:0]+1])/Tclk
**			定时器溢出时间	Tout = ((9999 + 1)*(7199 + 1))/72MHz = 1s
**			定时器溢出时间	Tout = ((19 + 1)*(7199 + 1))/72MHz = 2*1/1000s = 2ms
**********************************************************************/
void TIM2_Configuration(void)
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	htim2.Instance = TIM2;

	htim2.Init.Period            = 7200 - 1;
	htim2.Init.Prescaler         = 19;
	htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
	htim2.Init.RepetitionCounter = 0;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		while(1);
	}

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
	{
		while(1);
	}
}


/*************************************************************
**函数功能：定时器2中断控制器初始化配置
**************************************************************/
void TIM2_NVIC_Configuration(void)
{
    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 3);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}



void TIM_InitConfig(void)
{
	TIM2_NVIC_Configuration();
	TIM2_Configuration();
}






