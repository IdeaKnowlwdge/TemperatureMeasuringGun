#include "Voltage_Driver.h"


__IO uint32_t ADC_ConvertedValue;
DMA_HandleTypeDef DMA_Init_Handle;
ADC_HandleTypeDef ADC_Handle;
ADC_ChannelConfTypeDef ADC_Config;
uint16_t g_waRawADCVal[50];
uint16_t g_waAvgADCRawVal;

static void DMA_ADC_Config(void);
static void DMA_ADC_TransferCplt(DMA_HandleTypeDef * _hdma);

static void DMA_ADC_Config(void)
{
	HAL_StatusTypeDef  tErrCode;
  __HAL_RCC_DMA1_CLK_ENABLE();
	
	__HAL_LINKDMA(&ADC_Handle, DMA_Handle, DMA_Init_Handle);
  DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
  DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
  DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE;
  DMA_Init_Handle.Init.PeriphDataAlignment =DMA_PDATAALIGN_HALFWORD;
  DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
  DMA_Init_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;

  DMA_Init_Handle.Instance = DMA1_Channel1;

	HAL_DMA_DeInit(&DMA_Init_Handle);
	
	if (HAL_ERROR == HAL_DMA_Init(&DMA_Init_Handle))
	{
			while(1);
	}
	
  __HAL_DMA_ENABLE_IT(&DMA_Init_Handle, DMA_IT_TC);
	
	
	//设置中断优先级
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);	

	//HAL_DMA_RegisterCallback(&DMA_Init_Handle, HAL_DMA_XFER_CPLT_CB_ID, DMA_ADC_TransferCplt);
	
	
	tErrCode = HAL_DMA_Start_IT(&DMA_Init_Handle, (uint32_t)&ADC_Handle.Instance->DR, (uint32_t)g_waRawADCVal, sizeof(g_waRawADCVal)/sizeof(g_waRawADCVal[0]));	
	// 
}


static void DMA_ADC_TransferCplt(DMA_HandleTypeDef * _hdma)
{
  uint8_t cnt;
  uint64_t ulADCRawSum = 0;
  uint8_t ucBufferLen = sizeof(g_waRawADCVal) / sizeof(g_waRawADCVal[0]);
  uint16_t wValMax = 0;
	uint16_t wValMin = 0xFFFF;	
	
  for (cnt = 0; cnt < ucBufferLen; cnt++)
  {
		ulADCRawSum += g_waRawADCVal[cnt];
		if (wValMax <= g_waRawADCVal[cnt])
			wValMax = g_waRawADCVal[cnt];
    if (wValMin >= g_waRawADCVal[cnt])
			wValMin = g_waRawADCVal[cnt];
  }
	ulADCRawSum = ulADCRawSum - wValMax - wValMin;
	
  g_waAvgADCRawVal = (uint16_t)(ulADCRawSum / (ucBufferLen - 2));
}

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

    //
		DMA_ADC_Config();
		
		//HAL_ADC_Start(&ADC_Handle);
    //HAL_ADC_Start_DMA(&ADC_Handle, (unsigned int*)g_waRawADCVal, 50);
    //HAL_ADC_Start_IT(&ADC_Handle);
}

// 配置中断优先级
static void Voltage_ADC_NVIC_Config(void)
{
  HAL_NVIC_SetPriority(Voltage_ADC_IRQ, 0, 0);
  HAL_NVIC_EnableIRQ(Voltage_ADC_IRQ);
}

void Voltage_Init(void)
{
	Voltage_ADC_GPIO_Config();
	Voltage_ADC_Mode_Config();
  //Voltage_ADC_NVIC_Config();

	HAL_ADC_Start_DMA(&ADC_Handle,(unsigned int*)g_waRawADCVal, sizeof(g_waRawADCVal)/sizeof(g_waRawADCVal[0]));
}

/**
  * @brief  转换完成中断回调函数（非阻塞模式）
  * @param  AdcHandle : ADC句柄
  * @retval 无
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  /* 获取结果 */
  //ADC_ConvertedValue = HAL_ADC_GetValue(AdcHandle);
  uint8_t cnt;
  uint64_t ulADCRawSum = 0;
  uint8_t ucBufferLen = sizeof(g_waRawADCVal) / sizeof(g_waRawADCVal[0]);
  
  for (cnt = 0; cnt < ucBufferLen; cnt++)
  {
    ulADCRawSum += g_waRawADCVal[cnt];
  }

  g_waAvgADCRawVal = (uint16_t)(ulADCRawSum / ucBufferLen);
	
	
}



/***************************************
**函数功能：获取采集电压值
**公	式：V(sample) = V(REF) * Value/(0x0FFF + 1)
****************************************/
float Get_VoltageValue(void)
{
	float Value;
 //(float) ADC_ConvertedValue/4096*(float)3.3; // 读取转换的AD值
	Value = (float)g_waAvgADCRawVal * ((float)3.3 / 4096);

	return Value;
}

