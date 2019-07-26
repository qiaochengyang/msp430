#ifndef _SPI_H_
#define _SPI_H_
#include "msp430.h"

#define CS_H P5OUT |= BIT0
#define CS_L P5OUT &= ~BIT0

#define MOSI_H P8OUT |= BIT5
#define MOSI_L P8OUT &= ~BIT5

#define SCLK_H P8OUT |= BIT4
#define SCLK_L P8OUT &= ~BIT4

#define MISO ((P8IN&BIT6)>>5)

unsigned char SPI_SendByte(unsigned char dt);
void SPI_CS(unsigned char status);
void SPI_IO_INIT(void);

#endif
