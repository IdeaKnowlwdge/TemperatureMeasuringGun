#ifndef _PIPE_H
#define _PIPE_H

#include "stm32f1xx.h"
#include <stdbool.h>

typedef struct _Pipe_t {
	uint8_t * beginPtr;	//管道内存块首地址
	uint8_t * headPtr;		//管道元素首部,指向下一个要存储的地址
	uint8_t * tailPtr;		//管道元素尾部,指向下一个要读取的地址
	uint8_t * endPtr;	//管道内存块尾部 ,最后一个内存地址+1
}  Pipe_t;

void pipe_init(Pipe_t *pipe, uint8_t *buffer, uint16_t size);
bool pipe_write(Pipe_t *pipe, uint8_t value);
bool pipe_read(Pipe_t *pipe, uint8_t *value);

#endif
