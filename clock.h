#ifndef __CLOCK_H
#define __CLOCK_H
#include <stdint.h>

#define XT2_FREQ   4000000

#define MCLK_FREQ 16000000
#define SMCLK_FREQ 4000000

#define PMM_STATUS_OK     0
#define PMM_STATUS_ERROR  1
#define _HAL_PMM_SVMLE  (SVMLE)
#define _HAL_PMM_SVSLE  (SVSLE)
#define _HAL_PMM_SVSFP  (SVSLFP)
#define _HAL_PMM_SVMFP  (SVMLFP)
uint16_t SetVCore (uint8_t level);
static uint16_t SetVCoreUp (uint8_t level);
void clockInit(void);
void delay_ms(uint16_t m);
void delay_us(uint16_t m);
void initClock();
#endif