#include "pipe.h"

void pipe_init(Pipe_t *pipe, uint8_t * buffer, uint16_t size)
{	
	(*pipe).beginPtr = buffer;
	(*pipe).endPtr = buffer+size;
	(*pipe).headPtr = buffer;
	(*pipe).tailPtr = buffer;
}

bool pipe_read(Pipe_t *pipe, uint8_t * value)
{
	if((*pipe).headPtr != (*pipe).tailPtr)
	{
		*value = *((*pipe).tailPtr);
		(*pipe).tailPtr++;
		if((*pipe).tailPtr == (*pipe).endPtr)
			(*pipe).tailPtr = (*pipe).beginPtr;
		return true;
	}
	else
	{
		*value = 0xFF;
		return false;
	}
}

bool pipe_write(Pipe_t *pipe, uint8_t value)
{
	if((*pipe).headPtr != (*pipe).tailPtr - 1)
	{
		if(((*pipe).headPtr != (*pipe).endPtr -1) || ((*pipe).tailPtr != (*pipe).beginPtr))
		{
			*((*pipe).headPtr) = value;
			(*pipe).headPtr++;
			if((*pipe).headPtr == (*pipe).endPtr)
			{
				(*pipe).headPtr = (*pipe).beginPtr;
			}
			return true;
			
		}
		else
		{
			return false;
		}
		
	}
	else
	{
		return false;
	}
}

