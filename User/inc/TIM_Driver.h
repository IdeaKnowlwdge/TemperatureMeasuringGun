#ifndef __TIM_DRIVER_H__
#define __TIM_DRIVER_H__

#include "stm32f10x.h"


void Tim2_NVIC(void);
void Tim2_Init(void);


void TIM2_Configuration(void);
void TIM2_NVIC_Configuration(void);
void TIM4_Configuration(void);
void TIM4_NVIC_Configuration(void);
void TIM_InitConfig(void);

#endif
