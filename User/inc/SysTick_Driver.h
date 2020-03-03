#ifndef __SYSTICK_DRIVER_H_
#define __SYSTICK_DRIVER_H_

#include "stm32f1xx.h"


void SysTick_Init(void);
void delay_us(__IO uint32_t nTime);

#define delay_ms(x) delay_us(100*x)	 //µ¥Î»ms
void TimingDelay_Decrement(void);


#endif


