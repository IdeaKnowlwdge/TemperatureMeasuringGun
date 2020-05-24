#ifndef _SEQQUEUE_H_
#define _SEQQUEUE_H_


typedef unsigned char SeqQueue;
/*
*创建一个顺序队列 
*/
SeqQueue* SeqQueue_Create(void);

/*
*销毁顺序队列 
*/
void SeqQueue_Destroy(SeqQueue* queue);

/*
*清空顺序队列 
*/
void SeqQueue_Clear(SeqQueue* queue);

/*
*向顺序队列插入新元素 
*/
int SeqQueue_Append(SeqQueue* queue, unsigned char item);

/*
*删除队列元素 
*/
unsigned char SeqQueue_Retrieve(SeqQueue* queue);

/*
*读取队列元素 
*/ 
unsigned char SeqQueue_Header(SeqQueue* queue);

/*
*读取队列长度 
*/
int SeqQueue_Length(SeqQueue* queue);

/*
*读取队列容量 
*/
int SeqQueue_Capacity(SeqQueue* queue);


#endif







