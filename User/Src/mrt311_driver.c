#include "mrt311_driver.h"
#include "math.h"


//#define DEBUG_USART1      //需要调试则打开查看调试信息

#define NTC_SAMPLE_CHANEL		ADC_IN1

#define NTC_SAMPLE_H_CHANEL		ADC_IN1
#define NTC_SAMPLE_L_CHANEL		ADC_IN2

#define IR_SAMPLE_CHANEL_VIR	ADC_IN3
#define IR_SAMPLE_CHANEL_AIN04	ADC_IN4

#define SENSOR_CVCC				2.5		//传感器工作电压(单位: V)
#define R17						100		//R17电阻阻值(单位: K)

#define SENSOR_PWR_PORT			GPIOB
#define SENSOR_PWR_PIN			GPIO_PIN_10
#define SENSOR_PWR_CLK_EN()		__HAL_RCC_GPIOB_CLK_ENABLE()


void IR_Sensor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
    SENSOR_PWR_CLK_EN();
														   
    GPIO_InitStruct.Pin = SENSOR_PWR_PIN;	
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SENSOR_PWR_PORT, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(SENSOR_PWR_PORT, SENSOR_PWR_PIN, GPIO_PIN_SET);
}

void IR_Sensor_Enable(void)
{
	HAL_GPIO_WritePin(SENSOR_PWR_PORT, SENSOR_PWR_PIN, GPIO_PIN_RESET);
}

void IR_Sensor_Disble(void)
{
	HAL_GPIO_WritePin(SENSOR_PWR_PORT, SENSOR_PWR_PIN, GPIO_PIN_SET);
}

/***************************************
**函数功能：获取NTC采集电压值
****************************************/
float Get_NTC_Sample_Vol(void)
{
	return Get_AdcMath(Get_Adc(NTC_SAMPLE_CHANEL));
}

/***************************************
**函数功能：获取采集电压值
**公	式：CVCC/(R17+R(NTC)) = V(sample)/R(NTC)
**			R(NTC) = V(sample)*R17/(CVCC-V(sample)) (K)
****************************************/
float Get_R_Val(void)
{
	float R_NTC = 0;
	float V_sample = Get_NTC_Sample_Vol();

	R_NTC = (V_sample * R17) / (SENSOR_CVCC - V_sample);
//	printf("Get_R_Val: R_NTC = %f\r\n",R_NTC);
	return (R_NTC);
}

/***************************************
**函数功能：获取采集电压值
**公	式：V(NTCH)/(R(NTC)+R18) = V(NTCL)/R18
**			R(NTC) = V(NTCH)*R18/V(NTCL) - R18 (K)
****************************************/
float Get_R_Val_1(void)
{
	float R_NTC = 0;
	float V_NTCH = Get_AdcMath(Get_Adc(NTC_SAMPLE_H_CHANEL));
	float V_NTCL = Get_AdcMath(Get_Adc(NTC_SAMPLE_L_CHANEL));
	float R18 = 47;
	
	R_NTC = V_NTCH*R18/V_NTCL - R18;
//	printf("Get_R_Val_1: R_NTC = %f\r\n",R_NTC);
	return (R_NTC);
}

/***************************************
**函数功能：获取NTC温度值,取整数温度
**公	式：查表 NTC_R_T_tab
****************************************/
float Get_NTC_T_Val(void)
{
	int i = 0;
	float R_val = Get_R_Val_1();
	float T_val = 0;
	
	for(i = 0;i < NTC_TEMP_TAB;i++)
	{
		if((R_val >= NTC_R_T_tab[i+1][NTC_R])&&(R_val <= NTC_R_T_tab[i][NTC_R]))
		{
			break;
		}
	}
	
	if(i >= NTC_TEMP_TAB)
		return -1;
	
	T_val = NTC_R_T_tab[i][NTC_T];
	
	return T_val;
}

/***************************************
**函数功能：获取NTC温度值,精确到小数点1位
**公	式：查表 NTC_R_T_tab
****************************************/
float Get_NTC_Temp(void)
{
	int i = 0;
	float R_val = Get_R_Val_1();
	float T_val = 0;
	
	for(i = 0;i < NTC_TEMP_TAB;i++)
	{
		if((R_val >= NTC_R_T_tab[i+1][NTC_R])&&(R_val < NTC_R_T_tab[i][NTC_R]))
		{
			break;
		}
	}
	
	if(i >= NTC_TEMP_TAB)
		return -1;
	
	T_val = NTC_R_T_tab[i][NTC_T];
	//根据 NTC_R_T_tab[i][NTC_R]-NTC_R_T_tab[i+1][NTC_R] 的差计算温度刻度，
	//计算NTC温度的小数，简单建模
	float base_val = NTC_R_T_tab[i][NTC_R];
	float diff_val = NTC_R_T_tab[i][NTC_R] - NTC_R_T_tab[i+1][NTC_R];
	float samp_val = diff_val/10;
	 

	for(i = 1;i <= 10;i++)
	{
		if(R_val >= (base_val - samp_val*i))
		{
			break;
		}
	}
	
	if(i > 10)
		return -1;
	
	T_val = T_val + (float)(i-1)/10;
	
#ifdef DEBUG_USART1		
	printf("T_val = %f\n",T_val);
#endif
	
	return T_val;
}



const float Rp=100000.0; //100K
const float T2 = (273.15+25.0);//T2
const float Bx = 3950.0;//B
const float Ka = 273.15;



float Get_NTC_Temp1(void)
{
	float Rt;
	float temp;
	Rt = Get_R_Val_1() * 1000;
	//like this R=5000, T2=273.15+25,B=3470, RT=5000*EXP(3470*(1/T1-1/(273.15+25)),  
	temp = Rt/Rp;
	temp = log(temp);//ln(Rt/Rp)
	temp/=Bx;//ln(Rt/Rp)/B
	temp+=(1/T2);
	temp = 1/(temp);
	temp-=Ka;
	
	return temp;
}


/***************************************
**函数功能：获取红外测温采集电压值
****************************************/
float Get_IR_Sample_Vol(void)
{
	return Get_AdcMath(Get_Adc(IR_SAMPLE_CHANEL_VIR));
}

/***************************************
**函数功能：获取红外测温端电压值(单位: uV)
**公	式：单位 V 转换为 uV
**			1V=1000mV 1mV=1000uV
**			放大器放大了300倍
****************************************/
uint32_t Get_IR_Vol(void)
{
	

	uint32_t uV = Get_AdcMath_uV(Get_Adc(IR_SAMPLE_CHANEL_VIR));
	uint32_t uV_ref = Get_AdcMath_uV(Get_Adc(IR_SAMPLE_CHANEL_AIN04));

	
//  return ((uV/300)*((float)uV_ref/(3.3*1000000)));
	
	return (uV/300);
}


/***************************************
**函数功能：获取红外测温温度值
**公	式：查表 IR_V_T_tab
****************************************/
float Get_Tobj_VRange(void)
{
	int i = 0;
	float IR_Temp = 0.0;
	int Tamb_val = Get_NTC_T_Val();
	uint16_t IR_Vol = Get_IR_Vol();

	
	for(i = 0;i < IR_VC_RANGE_MXA;i++)
	{
		if(Tamb_val == (NTC_REF_TEMP + i))
		{
			IR_Vol = IR_Vol - IR_Voltage_Calibration_tab[i];
			
#ifdef DEBUG_USART1			
			printf("IR_Vol21 = %d\n",IR_Vol);
			printf("Tamb_val = %d\r\n",Tamb_val);
			printf("test : %d\n",IR_V_T_tab[0][Tamb_val]);
#endif		
			
			if(IR_Vol < IR_V_T_tab[0][Tamb_val])
			{
				//体温小于预设的测试量程,预测最最低测量35°C
				printf("没有检测物体...\n");
				return -1;
			}
			
		}
		
	}
	
	 //如果环境温度大于等于36摄氏度后，不可以获取黑体或人体温度
	if(Get_NTC_T_Val() > (IR_AMB_TEMP_RANGE_MXA-1))	//最大测试环境温度为36°C,环节温度为 0~42°C
		return -1;
	
	//人体测试温度设定为 35 - 42℃ （8个刻度）
	for(i = 0;i < IR_TEMP_RANGE_MXA;i++)
	{
		//通过查表算法进行获取目标温度值
		if((IR_Vol >= IR_V_T_tab[i][Tamb_val])&&(IR_Vol < IR_V_T_tab[i+1][Tamb_val]))
		{
			break;
		}
		
	}
	
	if(i >= IR_TEMP_RANGE_MXA)
	{
		//体温大于预设的测试量程,预测最高测量42°C
		if(IR_Vol > IR_V_T_tab[IR_TEMP_RANGE_MXA-1][Tamb_val])
			IR_Temp = IR_TemperatureRange_tab[IR_TEMP_RANGE_MXA - 1];
		else return -1;
	}
	else
		IR_Temp = IR_TemperatureRange_tab[i];
	
	//根据 IR_TemperatureRange_tab[i+1]-IR_TemperatureRange_tab[i]的差值来计算红外温度刻度
	//计算红外测温温度的小数,精确到小数点后1位,简单建模
	float base_val = IR_V_T_tab[i][Tamb_val];
	float diff_val = IR_V_T_tab[i+1][Tamb_val] - IR_V_T_tab[i][Tamb_val];
	float samp_val = diff_val/10;
	
	for(i = 1;i <= IR_TEMP_RANGE_MXA;i++)
	{
		if(IR_Vol < (base_val + samp_val*i))
		{
			break;
		}
	}
	
	if(i > IR_TEMP_RANGE_MXA)
		return -1;
	
	IR_Temp = IR_Temp + (float)(i-1)/10;

#ifdef DEBUG_USART1		
	printf("人体温度整数.小数: %f\n",IR_Temp);
#endif
	
	return IR_Temp;
}


/***************************************
**函数功能：获取红外测温温度系数
**公	式：s = ((V(Tobj=37))/((273.15+37)^4 - (T(amb)+273.15)^4))/(5e-10*(1+2e-3*T(amb)))
****************************************/
float Get_IR_coefficient(float T_amb)
{
	uint32_t V_obj_37 = 1062;
//	float T_amb = Get_NTC_Temp();
	float s = 0.0;
//	float e = 2.71828;
	
//s = ((V_obj_37)/(pow(273.15+37,4) - pow(T_amb + 273.15,4)))/(5*exp(1)-10*(1+2*exp(1)-3*T_amb));
	s = ((V_obj_37)/(pow(273.15+37,4) - pow(T_amb + 273.15,4)))/(5e1-10*(1+2e1-3*T_amb));
	
#ifdef DEBUG_USART1			
	printf("s = %f\n",s);
#endif
	
	return s;
}

/***************************************
**函数功能：获取红外测温温度值
**公	式：T(obj) = (V/(s*5e - 10*(1+2e - 3*T(amb)) + (T(amb)+273.15)^4)^0.25) - 273.15
****************************************/
float Get_Tobj_VRange_1(float T_amb,float s)
{
	float T_obj = 0.0;
//	float T_amb = Get_NTC_Temp();
	uint32_t uV = Get_IR_Vol();
	
//	float e = 2.71828;
	
//	T_obj = (uV/pow(s*5*exp(1) - 10*(1+2*exp(1) - 3*T_amb) + pow(T_amb+273.15,4),0.25));
	T_obj = (uV/pow((s)*5e1 - 10*(1+2e1 - 3*T_amb) + pow(T_amb+273.15,4),0.25));
	return T_obj;
}


void IR_test_demo(void)
{
	while(1)
	{
		
		Get_Tobj_VRange();
	
		delay_ms(1000);
		delay_ms(1000);
			
	}
}

