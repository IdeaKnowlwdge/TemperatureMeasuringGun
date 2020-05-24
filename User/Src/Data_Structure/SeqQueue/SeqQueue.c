#include "SeqList.h"
#include "SeqQueue.h"

/*
*创建一个顺序队列 
*/
SeqQueue* SeqQueue_Create(void)
{
	return SeqList_Create();
}

/*
*销毁顺序队列 
*/
void SeqQueue_Destroy(SeqQueue* queue)
{
	SeqList_Destroy(queue);
}

/*
*清空顺序队列 
*/
void SeqQueue_Clear(SeqQueue* queue)
{
	SeqList_Clear(queue);
}

/*
*向顺序队列插入新元素 item
*/
int SeqQueue_Append(SeqQueue* queue, unsigned char item)
{
	return SeqList_Insert(queue,item,SeqList_Length(queue));
}

/*
*删除队列元素 
*/
unsigned char SeqQueue_Retrieve(SeqQueue* queue)
{
	return SeqList_Delete(queue,0);
}

/*
*读取队列元素 
*/ 
unsigned char SeqQueue_Header(SeqQueue* queue)
{
	return SeqList_Get(queue,0);
}

/*
*读取队列长度 
*/
int SeqQueue_Length(SeqQueue* queue)
{
	return SeqList_Length(queue);
}

/*
*读取队列容量 
*/
int SeqQueue_Capacity(SeqQueue* queue)
{
	return SeqList_Capacity(queue);
}




