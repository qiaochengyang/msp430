/*
模拟spi
*/
#include "softSPI.h"
#include "clock.h"
/*****************************************************************
CPOL=0，表示当SCLK=0时处于空闲态，所以有效状态就是SCLK处于高电平时
CPHA=1，表示数据采样是在第2个边沿，数据发送在第1个边沿
*****************************************************************/

//CPOL=0  //CPHA=1
unsigned char SPI_SendByte(unsigned char dt)
{
    unsigned char i;
    unsigned char temp = 0;

    for (i = 0; i < 8; i++) {
        SCLK_L;
        //__delay_cycles(1);
        
        if (dt & 0x80) {
            MOSI_H;    //写数据
        }
        else {
            MOSI_L;
        }
        dt <<= 1;
        
        SCLK_H;
        //__delay_cycles(1);
        
        temp <<= 1;
        if (MISO) {
            temp |= 0x01;    //读数据
        }
    }

    return temp;
}
void SPI_CS(unsigned char status)
{
    if(status)
        CS_H;
    else
        CS_L;
     __delay_cycles(MCLK_FREQ/1000000*5);
}
void SPI_IO_INIT(void)
{
   P8DIR |= BIT4;
   P8OUT &= ~BIT4;
   P8DIR |= BIT5;
   P8DIR &= ~BIT6;
   P5DIR |= BIT0;
}
