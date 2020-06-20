#ifndef __MRT311_DRIVER_H__
#define __MRT311_DRIVER_H__

#include "include.h"

#define NTC_TEMP_R_NUM				2
#define NTC_TEMP_TAB				132
#define NTC_R						1
#define NTC_T						0
#define NTC_REF_TEMP		25

#define IR_TEMP_RANGE_MXA			8
#define IR_VC_RANGE_MXA			12
#define IR_TEMP_SCALE_MXA			71
#define IR_AMB_TEMP_RANGE_MXA		36





extern const float NTC_R_T_tab[NTC_TEMP_TAB][NTC_TEMP_R_NUM];
extern const float IR_TemperatureRange_tab[IR_TEMP_RANGE_MXA];
extern const uint32_t IR_Voltage_Calibration_tab[IR_VC_RANGE_MXA];
extern const float IR_TemperatureScale_tab[IR_TEMP_SCALE_MXA];
extern const int16_t IR_V_T_tab[IR_TEMP_RANGE_MXA][IR_AMB_TEMP_RANGE_MXA];


void IR_Sensor_Init(void);
void IR_Sensor_Enable(void);
void IR_Sensor_Disble(void);

void IR_test_demo(void);

float Get_Tobj_VRange(void);
float Get_NTC_T_Val(void);
float Get_NTC_Temp(void);
#endif

