#ifndef __VOLTAGE_DRIVER_H_
#define	__VOLTAGE_DRIVER_H_

#include "stm32f1xx.h"


typedef enum{
	ADC_IN1,
	ADC_IN2,
	ADC_IN3,
	ADC_IN4,
	ADC_BAT_VOLTAGE,
}__OWL_BOARD_ADC_CHANNEL;//GPIO口定义通道ADC通道的命名



void ADC_InitConfig(void);
uint16_t Get_Adc(uint8_t channel);
float Get_AdcMath(uint16_t adc_val);
uint32_t Get_AdcMath_uV(uint16_t adc_val);
float Get_BAT_Vol(void);
void Adc_test_demo(void);

#endif /* __VOLTAGE_DRIVER_H_ */


