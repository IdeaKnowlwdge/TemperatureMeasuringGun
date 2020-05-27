/*********************************iBoxV300*********************************                                      
 
	                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)           
***************************************************************************/
#include "ST_string.h"
#include <stdio.h>

/**************************************
**函数功能：获取字符串长度
***************************************/
signed int st_strlen(unsigned char* str)
{
	int i = 0;
	if(str != NULL)
	{
		while(*str++)
		{
			i++;
		}
	}
	
	return i;
}

/**************************************************
**函数功能：比较两个字符串大小
**************************************************/
signed int st_strcmp(unsigned char *s,unsigned char *t)
{
	while(*s == *t)
	{
		if(*s == 0)
		{
			return(0);
		}
	    ++s;
	    ++t;
	}
	
	return (*s - *t);
}


/***************************************************
**函数功能：字符串拷贝
****************************************************/
unsigned char *st_strcpy(unsigned char *t,unsigned char *s)
{
	unsigned char *d;

	d = t;
	while(*s)
	{
	   *t++ = *s++;
	}
	*t = 0x0;			// Add string terminator

	return(d);
}


/***********************************************************************
**函数功能：可变长度字符串大小比较
************************************************************************/
signed int st_strncmp(unsigned char *dest,unsigned char *src,unsigned int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++) 
	{
		if(dest[i] != src[i])
		{
		    if(dest[i] > src[i])
			{
		        return 1;
		    }
			else
			{
		        return -1;
		    }
	    }
	}

	return 0;

}

/*****************************************************
**函数功能：判断字符大小写
******************************************************/
signed char st_toupper(unsigned char c)
{
	if(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')))
	{
		c &= 0xDF;
	}
	
	return c;
}


/*********************************************************
**函数功能：字符串连接
**********************************************************/
unsigned char *st_strcat(unsigned char *s, unsigned char *t)
{
	unsigned char *d;
	
	d = s;
	--s;
	while(*++s);

	do 
	{
		*s = *t++;
	}
	while(*s++);

	return(d);
}


/*********************************************************************
**函数功能：可变长度内存拷贝
**********************************************************************/
signed int st_memcpy(unsigned char *dest, unsigned char *src, unsigned int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++)
	{
		dest[i] = src[i];
	}

	return(Len);
}


/***********************************************************************
**函数功能：设置长度为Len的内存块为byte
************************************************************************/
signed int st_memset(unsigned char *dest,unsigned char byte, unsigned int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++)
	{
		dest[i] = byte;
	}

	return(Len);
}



/***********************************************************************
**函数功能：内存段内数据比较
************************************************************************/
signed int st_memcmp(unsigned char* dest, unsigned char* src, unsigned int Len)
{
	unsigned int i;

	for(i = 0; i < Len; i++)
	{
		if(dest[i] != src[i])
		{
		    if(dest[i] > src[i])
			{
		        return 1;
		    }
			else
			{
		        return -1;
		    }
	    }
	}
	
	return 0;
}
/***********************************************************************
**函数功能：字符串转整数
************************************************************************/

int str_toint(unsigned char buf[])
{
	int sum = 0;
	if(st_strlen(buf) == 3)
	{
		sum = sum + (buf[0]-'0')*100;
		sum = sum + (buf[1]-'0')*10;
		sum = sum + (buf[2]-'0');	
	}
	
	if(st_strlen(buf) == 2)
	{
		if(buf[0] == '0')
		{
			sum = sum + (buf[1]-'0');
		}
		else
		{
			sum = sum + (buf[0]-'0')*10;
			sum = sum + (buf[1]-'0')*1;
		}
	}
	
	if(st_strlen(buf) == 4)
	{
		sum = sum + (buf[0]-'0')*1000;
		sum = sum + (buf[1]-'0')*100;
		sum = sum + (buf[2]-'0')*10;
		sum = sum + (buf[3]-'0');	}
	

	return sum;
}


#if 0
void strto_int(uint8_t buf[],int a[6])
{
	int i = 0;
	int sum = 0;
	sum = (buf[0]-'0')*1000;
	sum = sum + (buf[1]-'0')*100;
	sum = sum + (buf[2]-'0')*10;
	sum = sum + (buf[3]-'0');	
	a[0] = sum;		//year
	
	sum = 0;
	sum = sum + (buf[5]-'0')*10;
	sum = sum + (buf[6]-'0');
	a[1] = sum;	//month
	
	sum = 0;
	sum = sum + (buf[8]-'0')*10;
	sum = sum + (buf[9]-'0');
	a[2] = sum;	//day
	
	sum = 0;
	sum = sum + (buf[11]-'0')*10;
	sum = sum + (buf[12]-'0');
	a[3] = sum;	//hour
	
	sum = 0;
	sum = sum + (buf[14]-'0')*10;
	sum = sum + (buf[15]-'0');
	a[4] = sum;	//min
	
	sum = 0;
	sum = sum + (buf[17]-'0')*10;
	sum = sum + (buf[18]-'0');
	a[5] = sum;	//sec
	sum = 0;
}

#endif
