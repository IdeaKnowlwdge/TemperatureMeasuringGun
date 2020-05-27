#include "Data_Structure.h"


uint8_t iBox_work_mode = 0xff;



/***********************************************************************
*************************创建消息队列****************************
************************************************************************/
SeqQueue* queue_node_Fifo;



/**********************************************
**函数功能	:消息队列初始化
***********************************************/
void SeqQueue_Init(void)
{
	queue_node_Fifo = (uint8_t*)SeqQueue_Create();
}


/*************************************************************
**函数功能	:模式切换函数，伪造发消息
**************************************************************/
void put_msg_Fifo(uint8_t mode)
{
	SeqQueue_Append(queue_node_Fifo, mode);
//	iBox_work_mode = mode;
}

/************************************************************
**函数功能	:读取模式消息
*************************************************************/
#if 0
uint8_t get_msg_Fifo(void)
{
	iBox_work_mode = SeqQueue_Header(queue_node_Fifo);

	if(SeqQueue_Length(queue_node_Fifo) != 0)
		SeqQueue_Retrieve(queue_node_Fifo);
		
	return iBox_work_mode;
}
#else
uint8_t get_msg_Fifo(void)
{
	if(SeqQueue_Length(queue_node_Fifo) == 0)
		return 0xff;
		
	return SeqQueue_Retrieve(queue_node_Fifo);
}
#endif



/************************************************************
**函数功能	:获取消息队列中是否有消息
**返回	值	:0----无消息
*************************************************************/
uint8_t get_SeqQ_msg_Length(void)
{
	uint8_t ret = (uint8_t)SeqQueue_Length(queue_node_Fifo);
	return ret;
}


/************************************************************
**函数功能	:获取消息队列的容量
**返回	值	:0----无消息
*************************************************************/
uint8_t get_SeqQ_msg_Capacity(void)
{
	uint8_t ret = (uint8_t)SeqQueue_Capacity(queue_node_Fifo);
	return ret;
}


/************************************************************
**函数功能	:清空消息队列
*************************************************************/
void Clear_msg_Fifo(void)
{
	SeqQueue_Clear(queue_node_Fifo);
}




