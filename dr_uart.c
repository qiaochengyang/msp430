#include <msp430f6638.h>
#include "dr_uart.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "stdarg.h"
typedef uint8_t u8;

void UARTInit(void)	//RS232接口初始化函数
{
	P8SEL|=0x0c;	//模块功能接口设置，即P8.2与P8.3作为USCI的接收口与发射口
	UCA1CTL1|=UCSWRST;	//复位USCI
	UCA1CTL1|=UCSSEL__ACLK;	//设置辅助时钟，用于发生特定波特率
	UCA1BR0=0x03;		//设置波特率
	UCA1BR1=0x00;
	UCA1MCTL=UCBRS_3+UCBRF_0;
	UCA1CTL1&=~UCSWRST;	//结束复位
	UCA1IE|=UCRXIE;		//使能接收中断
}
 void UARTSendChar(u8 ch)
 {
    while (!(UCA1IFG & UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA1TXBUF =ch;   
 }