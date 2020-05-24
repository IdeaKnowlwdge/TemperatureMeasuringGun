#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

#include "stm32f1xx.h"

#include "SeqQueue.h"


extern uint8_t iBox_work_mode;


void SeqQueue_Init(void);

void put_msg_Fifo(uint8_t mode);

uint8_t get_msg_Fifo(void);
uint8_t get_SeqQ_msg_Length(void);
uint8_t get_SeqQ_msg_Capacity(void);
void Clear_msg_Fifo(void);

#endif

