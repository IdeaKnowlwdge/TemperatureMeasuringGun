#include "TIM_Driver.h"






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
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清中断，以免一启用中断后立即产生中断

	//TIM_PrescalerConfig(TIM2,1999,TIM_PSCReloadMode_Immediate); 
	TIM_ARRPreloadConfig(TIM2, DISABLE); 	
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
  TIM_Cmd(TIM2, ENABLE);	
}


/*************************************************************
**函数功能：定时器2中断控制器初始化配置
**************************************************************/
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void TIM_InitConfig(void)
{
	TIM2_Configuration();
	TIM2_NVIC_Configuration();

}






