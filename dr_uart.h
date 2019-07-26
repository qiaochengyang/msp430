#ifndef __DR_UART_H
#define __DR_UART_H
#include <stdint.h>
void UARTInit(void);
void UARTSendChar(uint8_t ch);
#endif