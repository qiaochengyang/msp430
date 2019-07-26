/*!
*  \file cmd_queue.h
*  \brief ����ָ�����
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
*  \brief  ���ָ������
*/
extern void queue_reset(void);

/*!
* \brief  ���ָ������
* \detial ���ڽ��յ����ݣ�ͨ���˺�������ָ�����
*  \param  _data ָ������
*/
extern void queue_push(qdata _data);

/*!
*  \brief  ��ָ�������ȡ��һ��������ָ��
*  \param  cmd ָ����ջ�����
*  \param  buf_len ָ����ջ�������С
*  \return  ָ��ȣ�0��ʾ������������ָ��
*/
//extern qsize queue_find_cmd(qdata *cmd, qsize buf_len);
u8 queue_find_cmd(DATA_TYPE *data_value, u8 data_len);
u8 Data_Receive_Prepare(u8 data);
DATA_TYPE* Data_Receive_Anl(u8 *data_buf,u8 num);
void send(u8 channel,int cnt,...);
void ESP_8266_Init(void);
#endif

