#include<stdio.h>
#include"SeqList.h"



typedef unsigned char TSeqListNode;

typedef struct _tag_SeqList
{
	int capacity;               //表示线性表最大容量
	int length;                 //表示线性表当前元素个数
	TSeqListNode* node;         //表示线性表的空间，所以用指针，而不用数组 
}TSeqList;

TSeqList Queue_Info;

unsigned char Queue_Info_buf[5] = {0};

/*
    该方法用于创建并且返回一个空的线性表
*/
SeqList* SeqList_Create(void)
{
	TSeqList* ret = NULL;
	

	ret = &Queue_Info;
	
	if( ret != NULL)
	{
		ret->capacity = 5;
		ret->length = 0;
		ret->node = Queue_Info_buf;
	}
	
	return ret;
}

/*
    该方法用于销毁一个线性表list
*/
void SeqList_Destroy(SeqList* list)
{
//	free(list);
}

/*
    该方法用于将一个线性表list中的所有元素清空
    使得线性表回到创建时的初始状态
*/
void SeqList_Clear(SeqList* list)
{
	TSeqList* sList = (TSeqList*)list;
	int ret = ( list != NULL );
	int i = 0;
	
	if(ret)
	{
		sList->length = 0;
		
		for(i = 0;i < sList->capacity;i++)
			sList->node[i] = 0xff;
	}
}

/*
    该方法用于返回一个线性表list中的所有元素个数
*/
int SeqList_Length(SeqList* list)
{
	int ret = -1;
	TSeqList* sList = (TSeqList*)list;
	
	if( list != NULL)
	{
		ret = sList->length;
	}
	
	return ret;
}

/*
	该方法用于返回线性表list中的最大容量 
*/
int SeqList_Capacity(SeqList* list)
{
	int ret = -1;
	TSeqList* sList = (TSeqList*)list;
	
	if( list != NULL)
	{
		ret = sList->capacity;
	}
	
	return ret;
}

/*
    该方法用于向一个线性表list的pos位置处插入新元素node
    返回值为1表示插入成功，0表示插入失败
*/
int SeqList_Insert(SeqList* list, SeqListNode node, int pos)
{
		//1.判断线性表是否合法
	int ret = (list != NULL);
	TSeqList* sList = (TSeqList*)list;
//	int i;
	
	//2.判断插入位置是否合法 
	ret = ret && ( sList->length + 1<= sList->capacity);
	ret = ret && (pos >= 0);
	
	if( ret )
	{
		if( pos >= sList->length )
		{
			pos = sList->length;
		}
		
		//3.把最后一个元素到插入位置的元素都向后移动一位
//		for(i = sList->length ; i > pos; i--)
//		{
//			sList->node[i] = sList->node[i-1];
//		} 
		
		 //4.将元素插入
//		 sList->node[i] = (TSeqListNode)node;
		 
		 sList->node[sList->length] = (TSeqListNode)node;
		 
		//5.线性表长度加1 
		sList->length++;
		if(sList->capacity < sList->length)
			sList->length = sList->capacity;
	}
	
	return ret;
}

/*
    该方法用于获取一个线性表list的pos位置处的元素
    返回值为pos位置处的元素，NULL表示获取失败
*/
SeqListNode SeqList_Get(SeqList* list, int pos)
{
	SeqListNode ret = 0xff;
	TSeqList* sList = (TSeqList*)list;
	
	if( (list != NULL) && (pos >= 0) && (pos < sList->length) )
	{
		ret = (TSeqListNode)(sList->node[pos]);
	}
	
	return ret;
}

/*
    该方法用于删除一个线性表list的pos位置处的元素
    返回值为被删除的元素，NULL表示删除失败
*/
SeqListNode SeqList_Delete(SeqList* list, int pos) 
{
	SeqListNode ret = SeqList_Get(list,pos);
	TSeqList* sList = (TSeqList*)list;
	int i;
	
	if( ret != 0)
	{
		//把pos+1个元素之后的所有元素都往前移动一位 
		for(i = pos +1;i < sList->length;i++)
		{
			sList->node[i-1] = sList->node[i];
		}
		
		if(sList->length > sList->capacity)
			sList->node[sList->length-1] = 0xff;
		else
			sList->node[sList->length] = 0xff;
			
		if(sList->length > 0)
			sList->length--;
	}
	
	return ret;
}





