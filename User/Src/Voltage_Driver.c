#include "Voltage_Driver.h"

#ifdef USE_DMA
#define CONVER_NO   50
__IO uint16_t ADC_ConvertedValue[CONVER_NO];
#else
__IO uint32_t ADC_ConvertedValue;
#endif

DMA_HandleTypeDef DMA_Handle;
ADC_HandleTypeDef ADC_Handle;
ADC_ChannelConfTypeDef ADC_Config;

static void Voltage_ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    VOLTAGE_ADC_CLK_ENABLE(); 
    // 使能 GPIO 时钟
    VOLTAGE_ADC_GPIO_CLK_ENABLE();
          
    // 配置 IO
    GPIO_InitStructure.Pin = VOLTAGE_ADC_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(VOLTAGE_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

static void Voltage_ADC_Mode_Config(void)
{
    RCC_PeriphCLKInitTypeDef ADC_CLKInit;
    // 开启ADC时钟
    ADC_CLKInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;			//ADC外设时钟
    ADC_CLKInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;			  //分频因子6时钟为72M/6=12MHz
    HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					      //设置ADC时钟
   
    ADC_Handle.Instance = VOLTAGE_ADC;
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;             //右对齐
    ADC_Handle.Init.ScanConvMode = DISABLE;                      //非扫描模式
    ADC_Handle.Init.ContinuousConvMode = ENABLE;                 //连续转换
    ADC_Handle.Init.NbrOfConversion = 1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;             //禁止不连续采样模式
    ADC_Handle.Init.NbrOfDiscConversion = 0;                     //不连续采样通道数为0
    ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;       //软件触发
    HAL_ADC_Init(&ADC_Handle);                                 //初始化 
 
 //---------------------------------------------------------------------------
    ADC_Config.Channel      = VOLTAGE_ADC_CHANNEL;
    ADC_Config.Rank         = 1;
    // 采样时间间隔	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_55CYCLES_5 ;
    // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为3个时钟周期
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
#ifdef USE_DMA
		Voltage_ADC_DMA_Config();
		HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)ADC_ConvertedValue, CONVER_NO);
#else
    HAL_ADC_Start_IT(&ADC_Handle);
#endif
}

#ifdef USE_DMA
//DMA搬运配置
static void Voltage_ADC_DMA_Config(void)
{
	VOLTAGE_ADC_DMA_CLK_ENABLE();

	DMA_Handle.Instance = VOLTAGE_ADC_DMA_CHANNEL;
	DMA_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;
	DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	DMA_Handle.Init.Mode = DMA_CIRCULAR;
	DMA_Handle.Init.Priority = DMA_PRIORITY_MEDIUM ;
	HAL_DMA_Init(&DMA_Handle);
	__HAL_LINKDMA(&ADC_Handle, DMA_Handle, DMA_Handle);
}
static uint16_t ADC_Get_Val(void)
{
	uint32_t sum = 0;
	uint16_t i;

	for (i=0; i<CONVER_NO; i++)
	{
		sum += ADC_ConvertedValue[i];
//		printf("%d  ",ADC_ConvertedValue[i]);
	}

	return (uint16_t)(sum/CONVER_NO);
}

#else

// 配置中断优先级
static void Voltage_ADC_NVIC_Config(void)
{
  HAL_NVIC_SetPriority(Voltage_ADC_IRQ, 0, 0);
  HAL_NVIC_EnableIRQ(Voltage_ADC_IRQ);
}
/**
  * @brief  转换完成中断回调函数（非阻塞模式）
  * @param  AdcHandle : ADC句柄
  * @retval 无
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  /* 获取结果 */
  ADC_ConvertedValue = HAL_ADC_GetValue(AdcHandle);
}

#endif

void Voltage_Init(void)
{
	Voltage_ADC_GPIO_Config();
	Voltage_ADC_Mode_Config();
#ifndef USE_DMA
  Voltage_ADC_NVIC_Config();
#endif
}




/***************************************
**函数功能：获取采集电压值
**公	式：V(sample) = V(REF) * Value/(0x0FFF + 1)
****************************************/
float Get_VoltageValue(void)
{
	float Value;
 //(float) ADC_ConvertedValue/4096*(float)3.3; // 读取转换的AD值
#ifdef USE_DMA
	Value = (float)ADC_Get_Val() * ((float)3.3 / 4096);
#else
	Value = (float)ADC_ConvertedValue * ((float)3.3 / 4096);
#endif
	return Value;
}


