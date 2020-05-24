#ifndef _SEQLIST_H_
#define _SEQLIST_H_

typedef void SeqList;
typedef unsigned char SeqListNode;

/*
    该方法用于创建并且返回一个空的线性表
*/
SeqList* SeqList_Create(void);

/*
    该方法用于销毁一个线性表list
*/
void SeqList_Destroy(SeqList* list);

/*
    该方法用于将一个线性表list中的所有元素清空
    使得线性表回到创建时的初始状态
*/
void SeqList_Clear(SeqList* list);

/*
    该方法用于返回一个线性表list中的所有元素个数
*/
int SeqList_Length(SeqList* list);

/*
	该方法用于返回线性表list中的当前元素位置 
*/
int SeqList_Capacity(SeqList* list); 

/*
    该方法用于向一个线性表list的pos位置处插入新元素node
    返回值为1表示插入成功，0表示插入失败
*/
int SeqList_Insert(SeqList* list, SeqListNode node, int pos);

/*
    该方法用于获取一个线性表list的pos位置处的元素
    返回值为pos位置处的元素，NULL表示获取失败
*/
SeqListNode SeqList_Get(SeqList* list, int pos);

/*
    该方法用于删除一个线性表list的pos位置处的元素
    返回值为被删除的元素，NULL表示删除失败
*/
SeqListNode SeqList_Delete(SeqList* list, int pos);

#endif



