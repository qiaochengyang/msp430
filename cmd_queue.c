/*
--------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------*/
#include "cmd_queue.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include <stdint.h>
#include "stdarg.h"
#include "dr_uart.h"	
#define QUEUE_MAX_SIZE 128
#define UART_CMD USART3
typedef struct _QUEUE {
    qsize _head;                                                       //队列头
    qsize _tail;                                                       //队列尾
    qdata _data[QUEUE_MAX_SIZE];                                       //队列数据缓存区
} QUEUE;

static QUEUE que = {0, 0, 0};                                          //指令队列

/*!
*  \brief  清空指令数据
*/
void queue_reset()
{
    que._head = que._tail = 0;
}
/*!
* \brief  添加指令数据
* \detial 串口接收的数据，通过此函数放入指令队列
* \param  _data 指令数据
*/
void queue_push(qdata _data)
{
    qsize pos = (que._head + 1) % QUEUE_MAX_SIZE;

    if (pos != que._tail) {                                           //非满状态
        que._data[que._head] = _data;
        que._head = pos;
    }
}

//从队列中取一个数据
static void queue_pop(qdata *_data)
{
    if (que._tail != que._head) {                                     //非空状态
        *_data = que._data[que._tail];
        que._tail = (que._tail + 1) % QUEUE_MAX_SIZE;
    }
}

//获取队列中有效数据个数
static qsize queue_size()
{
    return ((que._head + QUEUE_MAX_SIZE - que._tail) % QUEUE_MAX_SIZE);
}
DATA_TYPE* data_addr;
/*!
*  \brief  从指令队列中取出一条完整的指令
*  \param  cmd 指令接收缓存区
*  \param  buf_len 指令接收缓存区大小
*  \return  指令长度，0表示队列中无完整指令
*/
u8 queue_find_cmd(DATA_TYPE *data_value, u8 data_len)
{
		u8 data_size;
    qdata buf = 0;

    while (queue_size() > 0) {
        //取一个数据
        queue_pop(&buf);
		data_size=Data_Receive_Prepare(buf);
		if(data_size>0&&data_size<data_len)
		{
				memcpy(data_value,data_addr,data_size*sizeof(DATA_TYPE));
				free(data_addr);
				return data_size;
		}
    }
    return 0;                                                         //没有形成完整的一帧
}
u8 Data_Receive_Prepare(u8 data)
{
	static u8 RxBuffer[50];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAA)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		data_addr=Data_Receive_Anl(RxBuffer,_data_cnt+5);
		if(data_addr!=0)
			return RxBuffer[3]/sizeof(DATA_TYPE);
		
	}
	else
		state = 0;
	return 0;
}

DATA_TYPE * Data_Receive_Anl(u8 *data_buf,u8 num)
{
	u8 sum = 0;
  const int size=sizeof(DATA_TYPE);
	union   
	{  
	u8 byte[sizeof(DATA_TYPE)];  
	DATA_TYPE  value;  
	}v_b;
	int i,j;
	int cnt;
	DATA_TYPE *data_value;
	for(u8 i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return 0;		//判断sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAA))		return 0;		//判断帧头
  cnt=(num-5)/size;
	data_value=(DATA_TYPE*)malloc(num-5);
	if(!(data_value)) 
			return 0;
	for(i=0;i<cnt;i++)
	{
		for(j=0;j<size;j++)
			v_b.byte[j]=data_buf[size*(i+1)+3-j];
		*(data_value+i)=v_b.value;
	}
	
	return data_value;
}

void send(u8 channel,int cnt,...){
const int size=sizeof(DATA_TYPE);
union   
{  
u8 byte[sizeof(DATA_TYPE)];  
DATA_TYPE  value;  
}v_b;
int i,j;

u8 str[size*cnt+5];
va_list ap;
va_start(ap,cnt);
str[0]=0xAA;
str[1]=0xAA;
str[2]=channel;
str[3]=cnt*size;
str[size*cnt+4]=0;
for(i=0;i<cnt;++i)
{
	v_b.value=va_arg(ap,DATA_TYPE);
	for(j=0;j<size;j++)
	str[size*(i+1)+3-j]=v_b.byte[j];
}
    va_end(ap);
for(i=0;i<size*cnt+4;i++)
{
	str[size*cnt+4]=(str[size*cnt+4]+str[i])&0xff;	
	UARTSendChar ((u8) str[i]); 
}
UARTSendChar( (u8) str[size*cnt+4]); 
}
