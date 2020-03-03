#ifndef __MLX90614_DRIVER_H_
#define __MLX90614_DRIVER_H_


#include "stm32f1xx.h"
#include "main.h"
#include "stdint.h"



//#define ACK           0
//#define NACK          1
//#define SA            0x00 //Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
//#define RAM_ACCESS    0x00 //RAM access command
//#define EEPROM_ACCESS 0x20 //EEPROM access command
//#define RAM_TOBJ1     0x07 //To1 address in the eeprom



void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Delay(u16);
void SMBus_Init(void);

u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(void); //获取温度值


#endif



