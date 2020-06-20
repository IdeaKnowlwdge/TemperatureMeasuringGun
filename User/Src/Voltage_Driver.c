#include "Voltage_Driver.h"


/******************************************************************
**ADC端口:
**采集端口:
**A1------ADC1_IN1-----PA1
**A2------ADC1_IN2-----PA2
**A3------ADC1_IN3-----PA3
**A4------ADC1_IN4-----PA4
**voltage-ADC1_IN7-----PA7
*******************************************************************/

__IO uint16_t ADC_ConvertedValue[50][5];//用来存放ADC转换结果，也是DMA的目标地址
__IO uint16_t After_filter[5];    //用来存放求平均值之后的结果

DMA_HandleTypeDef hdma_adc1;
ADC_HandleTypeDef hadc1;

static void ADC1_GPIO_Config(void)
{
	 /**ADC2 GPIO Configuration    
    PA1     ------> ADC2_IN1
    PA2     ------> ADC2_IN2
    PA3     ------> ADC2_IN3
    PA4     ------> ADC2_IN4
    PA7     ------> ADC2_IN7
    */
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 配置 IO
	GPIO_InitStructure.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

static void ADC1_DMA_Config(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	hdma_adc1.Instance = DMA1_Channel1;
	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;;            
	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;                 //外设增量模式
	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;                     //存储器增量模式 
	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc1.Init.Mode= DMA_CIRCULAR;                         //循环
	hdma_adc1.Init.Priority=DMA_PRIORITY_HIGH;               //高优先级

	//初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
	{
		while(1);
	}

	__HAL_LINKDMA( &hadc1,DMA_Handle,hdma_adc1);
}

static void ADC1_Mode_Config(void)
{
	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
	ADC_ChannelConfTypeDef sConfig;
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	
	// 开启ADC时钟
	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC外设时钟
	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV8;			  //分频因子6时钟为72M/8=9MHz
	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					      //设置ADC时钟

	hadc1.Instance = ADC1;
	hadc1.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
	hadc1.Init.ScanConvMode=ADC_SCAN_ENABLE;                      //非扫描模式
	hadc1.Init.ContinuousConvMode=ENABLE;                 //连续转换
	hadc1.Init.NbrOfConversion=5;                         //5个通道在转换
	hadc1.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
	hadc1.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		while(1);
	}
	
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		while(1);
	}
	
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = 2;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		while(1);
	}
	
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = 3;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		while(1);
	}
	
	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = 4;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		while(1);
	}
	
	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = 5;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		while(1);
	}
	
	if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_ConvertedValue, 50*5) != HAL_OK)
	{
		/* Start Conversation Error */
		while(1);
	}
}

/*****************************************************
**函数功能：ADC1初始化
******************************************************/
void ADC_InitConfig(void)
{
	ADC1_GPIO_Config();
	ADC1_DMA_Config();
	ADC1_Mode_Config();
}


//求平均值函数
void filter(uint8_t num)
{
	int  sum = 0;
	uint8_t  count;    

	for(count=0;count<50;count++)
	{
		sum += 0xfff&ADC_ConvertedValue[count][num];
	}
	After_filter[num]=sum/50;
	
//	After_filter[num]=ADC_ConvertedValue[0][num];
}

/*
**函数功能：获取X通道AD值（0~4096）
**参数    ：channel---模拟口通道号
**参数取值：	1~4,5
*/
uint16_t Get_Adc(uint8_t channel)
{	
 	filter(channel);
	return 	After_filter[channel];	  	
}

/***************************************
**函数功能：获取采集电压值 (单位: V)
**公	式：V(sample) = V(REF) * Value/(0x0FFF + 1)
****************************************/
float Get_AdcMath(uint16_t adc_val)
{
	uint16_t temp = adc_val;
	float Value;

//	Value = temp * 3.3;
//	Value = Value / (0x0FFF + 1);


	Value = (float)temp * (3.3 / 4096);

	return Value;
}

/***************************************
**函数功能：获取采集电压值 (单位: uV)
**公	式：V(sample) = V(REF) * 1000000 * Value /(0x0FFF + 1)
****************************************/
uint32_t Get_AdcMath_uV(uint16_t adc_val)
{
	uint16_t temp = adc_val;
	uint32_t Value;
	float Vref = 0;

//	Value = temp * 3.3;
//	Value = Value / (0x0FFF + 1);

	Vref = 3.3 * 1000000;
	Value = temp * (Vref / 4096);
//	printf("Value = %f\n",temp * (Vref / 4096));
	return Value;
}


/***************************************
**函数功能：获取电池电压值
**公	式：V/(R24+R26) = V(sample)/R26
**			V =  V(sample)/R26 * (R24+R26)
**			R24 = 10K
**			R26 = 10K
****************************************/
float Get_BAT_Vol(void)
{
	float vol = 0.0;
	
	vol = (Get_AdcMath(Get_Adc(ADC_BAT_VOLTAGE))/10) * (10+10);
	
	return vol;
}



#include "include.h"
void Adc_test_demo(void)
{
	while(1)
	{
		printf("channel A1 adc_Val = %d    V = %f \r\n",Get_Adc(ADC_IN1),Get_AdcMath(Get_Adc(ADC_IN1)));
		printf("channel A2 adc_Val = %d    V = %f \r\n",Get_Adc(ADC_IN2),Get_AdcMath(Get_Adc(ADC_IN2)));
		printf("channel A3 adc_Val = %d    V = %f \r\n",Get_Adc(ADC_IN3),Get_AdcMath(Get_Adc(ADC_IN3)));
		printf("channel A4 adc_Val = %d    V = %f \r\n",Get_Adc(ADC_IN4),Get_AdcMath(Get_Adc(ADC_IN4)));
		printf("channel BAT_VOL adc_Val = %d    V = %f \r\n",Get_Adc(ADC_BAT_VOLTAGE),Get_AdcMath(Get_Adc(ADC_BAT_VOLTAGE)));
		delay_ms(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);
	}
}



