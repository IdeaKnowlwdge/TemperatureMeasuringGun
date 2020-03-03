#include "Beep_Driver.h"


/* 私有类型定义*/
TIM_HandleTypeDef htimx;

/**
  * 函数功能: 定时器硬件初始化配置
  * 输入参数: htim：定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被GENERAL_TIMx_Init函数调用
  */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance==GENERAL_TIMx)
  {  
    /* 定时器通道功能引脚端口时钟使能 */
    GENERAL_TIM_GPIO_RCC_CLK_ENABLE();

    /* 定时器通道3功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GENERAL_TIM_CH4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStruct);
  }
}

/**
  * 函数功能: 通用定时器初始化并配置通道PWM输出
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void Beep_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  
  htimx.Instance = GENERAL_TIMx;
  htimx.Init.Prescaler = GENERAL_TIM_PRESCALER;
  htimx.Init.Period = GENERAL_TIM_PERIOD;
	htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimx);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimx, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx, &sMasterConfig);
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.Pulse = 0;
  HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_4);

  HAL_TIM_MspPostInit(&htimx);
	
	HAL_TIM_PWM_Start(&htimx, TIM_CHANNEL_4);//通道4开始生成PWM信号
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==GENERAL_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    GENERAL_TIM_RCC_CLK_ENABLE();
    
    /* 配置定时器中断优先级并使能 */
    HAL_NVIC_SetPriority(GENERAL_TIM_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GENERAL_TIM_IRQn);
  }
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==GENERAL_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    GENERAL_TIM_RCC_CLK_DISABLE();
    /* 禁用定时器中断 */
    HAL_NVIC_DisableIRQ(GENERAL_TIM_IRQn);
  }
} 


/*******************************************
**	函数名 : Beep_VoiceRegulation
**	功  能 : 控制蜂鸣器的声音大小						
**	参  数 : 最大值不大于200，最小值不小于0，填整数		
**  返回值 : 无
********************************************/
void Beep_VoiceRegulation(uint8_t VoiceSize)
{

	TIM3-> CCR4 = VoiceSize;

}


