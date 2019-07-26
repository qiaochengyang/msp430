/*!
*  \file cmd_queue.h
*  \brief 串口指令队列
*  \version 1.0
*  \date 2012-2018
*  \copyright 
*/

#ifndef _CMD_QUEUE
#define _CMD_QUEUE
#include <stdint.h>
#define DATA_TYPE int16_t
typedef unsigned char qdata;
typedef unsigned short qsize;
typedef uint8_t u8;
/*!
*  \brief  清空指令数据
*/
extern void queue_reset(void);

/*!
* \brief  添加指令数据
* \detial 串口接收的数据，通过此函数放入指令队列
*  \param  _data 指令数据
*/
extern void queue_push(qdata _data);

/*!
*  \brief  从指令队列中取出一条完整的指令
*  \param  cmd 指令接收缓存区
*  \param  buf_len 指令接收缓存区大小
*  \return  指令长度，0表示队列中无完整指令
*/
//extern qsize queue_find_cmd(qdata *cmd, qsize buf_len);
u8 queue_find_cmd(DATA_TYPE *data_value, u8 data_len);
u8 Data_Receive_Prepare(u8 data);
DATA_TYPE* Data_Receive_Anl(u8 *data_buf,u8 num);
void send(u8 channel,int cnt,...);
void ESP_8266_Init(void);
#endif

