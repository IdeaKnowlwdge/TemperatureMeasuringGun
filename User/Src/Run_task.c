#include "Run_task.h"
#include "Data_Structure.h"
#include "include.h"
#include "bmp.h"
#include "ST_string.h"

//温度的单位 ℃
uint8_t TempCompany[][16]=
{
	0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00,
	0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00,/*"℃",0*/

};

enum TEMP_LEVEL{
	TEMP_NORMAL = 0,
	TEMP_ONE_LEVEL_HIGH,
	TEMP_SECOND_LEVEL_HIGH,
	TEMP_THREE_LEVEL_HIGH,
	TEMP_FOUR_LEVEL_HIGH,
};

enum VOL_LEVEL{
	VOL_NORMAL = 0,
	VOL_LOW,
};

struct TempMeasuringGun_type{
	uint32_t TempCleanScreenFlag;  //温度清屏的标志
	uint32_t VolCleanScreenFlag;  //电压清屏的标志
	uint8_t CollectionFlag;  //采集数据的标志
	uint8_t TempWarn;		//温度报警
	uint8_t TempWarn_count;	//温度报警计数器
	uint8_t VolWarn;		//温度报警
	uint8_t VolWarn_count;	//温度报警计数器
	float Temperature;    //温度数据变量（浮点型）
	float VBAT;
	char TempValue[30]; //温度值（字符串）
	char VoltageValueStr[30]; //电压值（字符串）
};

static void Run_Task(void)
{
	uint8_t msg = NO_MSG;
	float VoltageValue = 0.0;
	struct TempMeasuringGun_type board;
	
	OWL_BOARD_DEBUG("Run_Task\r\n");
	
	st_memset((unsigned char *)&board,0, sizeof(struct TempMeasuringGun_type));
	
	put_msg_Fifo(MSG_KEY_TYPE_LEFT);
	while(1)
	{
		msg = get_msg_Fifo();
		
		switch(msg)
		{
			case MSG_KEY_TYPE_UP:
				//上按键:  实现测温功能，采集MLX90614的数据，并在显示屏显示
				OWL_BOARD_DEBUG("MSG_KEY_TYPE_UP\r\n");
			
				if(board.CollectionFlag != 0)
				{
					if(board.TempCleanScreenFlag == 0)//进入模式时，只清屏一次
					{
						OLED_DataClear();
						board.TempCleanScreenFlag++;
					}
					
					OLED_ShowCHinese16x16(5*16,2,0,TempCompany);
					
					board.Temperature = infrared_ReadTemp();  //读取温度
					sprintf(board.TempValue,"%.1f", board.Temperature);     //浮点型转换成字符串
					OLED_ShowString(40,2,(uint8_t *)board.TempValue,16);   //显示温度
					
					//发热分为： 
					//低热 ：37.2～38℃； 中等度热：38．1～39℃：
					//高热：39．1～41℃； 超高热 ：41℃以上
					if(board.Temperature >= 37.2 && board.Temperature <= 38.0)
					{	//低烧预警
						board.TempWarn = TEMP_ONE_LEVEL_HIGH;
						board.TempWarn_count = 0;
					}
					else if(board.Temperature >= 38.1 && board.Temperature <= 39.0)
					{	//中烧预警
						board.TempWarn = TEMP_SECOND_LEVEL_HIGH;
						board.TempWarn_count = 0;
					}
					else if(board.Temperature >= 39.1 && board.Temperature <= 41.0)
					{	//高烧预警
						board.TempWarn = TEMP_THREE_LEVEL_HIGH;
						board.TempWarn_count = 0;
					}
					else if(board.Temperature > 41)
					{	//超高烧预警
						board.TempWarn = TEMP_FOUR_LEVEL_HIGH;
						board.TempWarn_count = 0;
					}
					
					board.VolCleanScreenFlag = 0; //清除电压清屏的标志
				}
				break;
			case MSG_KEY_TYPE_DOWN:
				//下按键:  实现采集电压功能，并在显示屏显示
				OWL_BOARD_DEBUG("MSG_KEY_TYPE_DOWN\r\n");
				if(board.CollectionFlag != 0)
				{
					if(board.VolCleanScreenFlag == 0)
					{
						OLED_DataClear() ;      //清除数据行的屏幕信息
						OLED_ShowChar(80,2,'V',16);
						board.VolCleanScreenFlag++;
					}
					
					VoltageValue = Get_VoltageValue();//采样电压
					board.VBAT = VoltageValue*(10 + 10)/10;//锂电池电压
					
					sprintf(board.VoltageValueStr,"%.2f", board.VBAT);      //浮点型转换成字符串
					OLED_ShowString(40,2,(uint8_t *)board.VoltageValueStr,16);   //显示温度
					
					if(board.VBAT < 3.0)
					{
						board.VolWarn = VOL_LOW;
						board.VolWarn_count = 0;
					}
				}
				break;
			case MSG_KEY_TYPE_LEFT:	//左键：进行后退，并显示首界面
				OWL_BOARD_DEBUG("MSG_KEY_TYPE_LEFT\r\n");
				OLED_Clear();  //清屏
				OLED_DrawBMP(0,0,128,8,Peacock);  //显示首界面
				
				board.CollectionFlag = 0;
				board.TempCleanScreenFlag = 0;
				board.VolCleanScreenFlag = 0;
				break;
			case MSG_KEY_TYPE_RIGHT:	//进入选择界面
				OWL_BOARD_DEBUG("MSG_KEY_TYPE_RIGHT\r\n");
				OLED_Clear();  //清屏
				OLED_DrawBMP(0,0,128,8,BMP1);  //显示首界面
				OLED_ShowString(0,2,"(T):up (V):dowm ",16);
			
				board.CollectionFlag++;
				board.TempCleanScreenFlag = 0;
				board.VolCleanScreenFlag = 0;
				break;
			case MSG_60MS:
				if(board.TempWarn == TEMP_FOUR_LEVEL_HIGH)
				{
					if((board.TempWarn_count == 0)||(board.TempWarn_count%2 == 0))
					{
						Beep_VoiceRegulation(10);
						 Led_Ctl( Red, ON );
					}
					else
					{
						Beep_VoiceRegulation(0);
						Led_Ctl( Red, OFF );
					}
					board.TempWarn_count++;
					if(board.TempWarn_count > 10)
					{
						board.TempWarn_count = 0;
						board.TempWarn = TEMP_NORMAL;
						Beep_VoiceRegulation(0);
						Led_Ctl( Red, OFF );
					}
				}
				break;
			case MSG_100MS:
				if(board.TempWarn == TEMP_THREE_LEVEL_HIGH)
				{
					if((board.TempWarn_count == 0)||(board.TempWarn_count%2 == 0))
					{
						Beep_VoiceRegulation(10);
						 Led_Ctl( Red, ON );
					}
					else
					{
						Beep_VoiceRegulation(0);
						Led_Ctl( Red, OFF );
					}
					board.TempWarn_count++;
					if(board.TempWarn_count > 10)
					{
						board.TempWarn_count = 0;
						board.TempWarn = TEMP_NORMAL;
						Beep_VoiceRegulation(0);
						Led_Ctl( Red, OFF );
					}
				}
				break;
			case MSG_200MS:
//				OWL_BOARD_DEBUG("MSG_200MS\r\n");
				if(board.TempWarn == TEMP_SECOND_LEVEL_HIGH)
				{
					if((board.TempWarn_count == 0)||(board.TempWarn_count%2 == 0))
					{
						Beep_VoiceRegulation(10);
						 Led_Ctl( Red, ON );
					}
					else
					{
						Beep_VoiceRegulation(0);
						Led_Ctl( Red, OFF );
					}
					board.TempWarn_count++;
					if(board.TempWarn_count > 10)
					{
						board.TempWarn_count = 0;
						board.TempWarn = TEMP_NORMAL;
						Beep_VoiceRegulation(0);
						Led_Ctl( Red, OFF );
					}
				}
				break;
			case MSG_HALF_SECOND:
//				OWL_BOARD_DEBUG("MSG_HALF_SECOND\r\n");
				if(board.TempWarn == TEMP_ONE_LEVEL_HIGH)
				{
					if((board.TempWarn_count == 0)||(board.TempWarn_count%2 == 0))
					{
						Beep_VoiceRegulation(10);
						 Led_Ctl( Red, ON );
					}
					else
					{
						Beep_VoiceRegulation(0);
						Led_Ctl( Red, OFF );
					}
					board.TempWarn_count++;
					if(board.TempWarn_count > 10)
					{
						board.TempWarn_count = 0;
						board.TempWarn = TEMP_NORMAL;
						Beep_VoiceRegulation(0);
						Led_Ctl( Red, OFF );
					}
				}
				
				if(board.VolWarn == VOL_LOW)
				{
					if((board.VolWarn_count == 0)||(board.VolWarn_count%2 == 0))
					{
						Beep_VoiceRegulation(10);
						 Led_Ctl( Green, ON );
					}
					else
					{
						Beep_VoiceRegulation(0);
						Led_Ctl( Green, OFF );
					}
					board.VolWarn_count++;
					if(board.VolWarn_count > 10)
					{
						board.VolWarn_count = 0;
						board.VolWarn = VOL_NORMAL;
						Beep_VoiceRegulation(0);
						Led_Ctl( Green, OFF );
					}
				}
				break;
			case MSG_800MS:
				break;
			default:
				break;
		}
	}
}

void Run_Start(void)
{
	printf("Run_Start\r\n");
	
	SeqQueue_Init();
	Clear_msg_Fifo();
	
	Run_Task();
}

