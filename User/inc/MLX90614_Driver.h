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


float infrared_ReadTemp(void);

#endif



