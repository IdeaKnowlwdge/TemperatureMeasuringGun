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
#include "bmp.h"

//温度的单位 ℃
uint8_t TempCompany[][16]=
{
	0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00,
	0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00,/*"℃",0*/

};

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	uint8_t i,j;

	en_key_type_t keyType;    //按键类型变量
	float Temperature = 0;    //温度数据变量（浮点型）
	char TempValue[80] = {0}; //温度值（字符串）
	char VoltageValueStr[80] = {0}; //电压值（字符串）
	uint32_t TempCleanScreenFlag = 0;  //温度清屏的标志
	uint32_t VolCleanScreenFlag = 0;  //电压清屏的标志
	uint8_t CollectionFlag = 0;  //采集数据的标志
	float VoltageValue = 0.0;     //Vsimple电压值变量
	float VBAT = 0.0;            //锂电池电压的变量值

	HAL_Init();      
	SystemClock_Config();    // 系统时钟初始化成72 MHz 
	NVIC_Configuration();    //中断组别的选择
	Led_Init();              //初始化RGB灯
	SysTick_Init();          //SYSTick的初始化
	USARTx_IintConfig();     //串口1的初始化
	printf("hello world!!\n");
	Key_Init();              //按键初始化
	Beep_Init();             //蜂鸣器初始化
	Sof_I2C_Init();
	power_ctl_register();
	board_power_ctl(PWR_OLED,PWR_ENABLE);
	board_power_ctl(PWR_INFRARED,PWR_ENABLE);
	power_pin_ctl();
	OLED_Init();			          //初始化OLED显示屏
	Voltage_Init();          //电压采集初始化
	SMBus_Init();             //初始化

	//启动无操作界面	
	OLED_DrawBMP(0,0,128,8,Peacock);
	printf("hello world!!\n");
	while(1)
	{

		if(Key_IsAnyButtonPress(&keyType) == KEY_ON)    //按键扫描
		{
		
			switch((uint8_t)keyType)
			{
				
				//上按键:  实现测温功能，采集MLX90614的数据，并在显示屏显示
				case EN_KEY_TYPE_UP:            //上按键
					
							if(CollectionFlag != 0)
							{
								
								//进入该模式，只清一次屏
								if(TempCleanScreenFlag == 0)
								{
									OLED_DataClear() ;
									
									TempCleanScreenFlag++;
								}
								
								//温度单位显示 （℃）
								for(i = 5;i < 6;i++)
								{
									j = i - 5;
									OLED_ShowCHinese16x16(i*16,2,j,TempCompany);			
								}
							
								Temperature = SMBus_ReadTemp();  //读取温度  	
								sprintf(TempValue,"%.1f", Temperature);     //浮点型转换成字符串
								OLED_ShowString(40,2,(uint8_t *)TempValue,16);   //显示温度
								
								//发热分为： 低热 ：37.2～38℃；中等度热：38．1～39℃：高热：39．1～41℃； 超高热 ：41℃以上
								//低烧预警
								if(Temperature >= 37.2 && Temperature <= 38.0)
								{
									  //喇叭和红灯预警
										for(i = 0;i < 5;i++)
										{
											Beep_VoiceRegulation(10);
											 Led_Ctl( Red, ON );
											delay_ms(500);
											
											Beep_VoiceRegulation(0);	
											Led_Ctl( Red, OFF );
											delay_ms(500);
										}
								}
								//中烧预警
								else if(Temperature >= 38.1 && Temperature <= 39.0)
								{
										for(i = 0;i < 5;i++)
										{
											 Beep_VoiceRegulation(10);
											 Led_Ctl( Red, ON );
											 delay_ms(300);
											 Beep_VoiceRegulation(0);
											 Led_Ctl( Red, OFF );
											 delay_ms(300);
											}
								}
							  
								//高烧预警
								else if(Temperature >= 39.1 && Temperature <= 41.0)
								{
										for(i = 0;i < 5;i++)
									 {
												
											Beep_VoiceRegulation(10);
											Led_Ctl( Red, ON );
											delay_ms(80);
											Beep_VoiceRegulation(0);
											Led_Ctl( Red, OFF );
											delay_ms(80);
										}
								}
								//超高烧预警
								else if(Temperature > 41)
								{
											for(i = 0;i < 5;i++)
											{
										     Beep_VoiceRegulation(10);
												 Led_Ctl( Red, ON );
										     delay_ms(50);
										     Beep_VoiceRegulation(0);
												 Led_Ctl( Red, OFF );
										     delay_ms(50);
											}
								}
								
								VolCleanScreenFlag = 0; //清除电压清屏的标志
							}

				break;
				
							
				//下按键:  实现采集电压功能，并在显示屏显示
				case EN_KEY_TYPE_DOWN:         //下按键
					
						if(CollectionFlag != 0)
						{	
								//进入该模式，只清一次屏
								if(VolCleanScreenFlag == 0)
								{
									OLED_DataClear() ;      //清除数据行的屏幕信息
									OLED_ShowChar(80,2,'V',16);
									VolCleanScreenFlag++;
									
								}
								
								
								VoltageValue = Get_VoltageValue();
								//由于板子在电压采集的电路中加入了电阻，所以在串联电路中，电阻起到的作用是:分压
								//故，锂电池的电压 VBAT = VoltageValue*（10 K + 10K）/10K

								VBAT = VoltageValue*(10 + 10)/10;
								
								sprintf(VoltageValueStr,"%.2f", VBAT);     //浮点型转换成字符串
								//由于板子在电压采集的电路中加入了电阻所以加1.2V
								sprintf(VoltageValueStr,"%.2f", (VoltageValue + 1.20));     //浮点型转换成字符串
								OLED_ShowString(40,2,(uint8_t *)VoltageValueStr,16);   //显示温度
								
								//低压预警
								if(VoltageValue < 1.5)
								{
											for(i = 0;i < 5;i++)   //闪烁5次
											{
													Beep_VoiceRegulation(10);    //喇叭提示
												  Led_Ctl( Green, ON );
													delay_ms(800);               //绿灯
													Beep_VoiceRegulation(0);
												  Led_Ctl( Green, OFF );
													delay_ms(800);
										 }
								}
								
								TempCleanScreenFlag = 0;    //清除温度显示的标志
						}
				break;
				
				
				//左键：进行后退，并显示首界面
				case EN_KEY_TYPE_LEFT:    
					
				  delay_us(10); 
					OLED_Clear();  //清屏
					delay_ms(500);   //延时500ms
					OLED_DrawBMP(0,0,128,8,Peacock);  //显示首界面
				
					CollectionFlag = 0;  //采集标志清空
					TempCleanScreenFlag	 = 0; //清空标志
				  VolCleanScreenFlag = 0; //清空标志
						
				break;
				
				
				//进入选择界面
				case  EN_KEY_TYPE_RIGHT:

					OLED_Clear();  //清屏
					delay_ms(500);   //延时500ms
					OLED_DrawBMP(0,0,128,8,BMP1);  //显示首界面
						
				 OLED_ShowString(0,2,"(T):up (V):dowm ",16);   //显示温度
					
				 CollectionFlag++;  //采集标志位++
				 TempCleanScreenFlag	 = 0; //清空标志
				VolCleanScreenFlag = 0; //清空标志
				break;
				
				default :				
					  ;			
			}
		}
		
	}
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
