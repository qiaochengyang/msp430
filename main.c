#include "msp430f6638.h"
#include <inttypes.h>
#include "clock.h"
#include "dr_lcdseg.h"
#include "dr_adc.h"
#include "dr_dc_motor.h"
#include "dr_step_motor.h"
#include "dr_uart.h"
#include "cmd_queue.h"
#include "stdio.h"
#include "dr_tft.h"
#include "LTM022A69B.h"

volatile uint16_t adc0=0;

int main(void)
{
  unsigned int i=1;
  int32_t val;
  WDTCTL = WDT_ADLY_250;
  SFRIE1 |= WDTIE;
// _BIS_SR(LPM0_bits + GIE);
 
  
  initClock();
  lcd_init();
//  lcd_draw_line(GREEN,40,20,300,40);
//  lcd_draw_line(GREEN,300,40,280,220);
//  lcd_draw_line(GREEN,280,220,20,200);
//  lcd_draw_line(GREEN,20,200,40,20);
//  delay_ms(3000);
  lcd_clear_screen(GREEN);
  lcd_display_string( "MSP430F6638 EVM",BLACK,GREEN,5,2);
  lcd_display_string( "TEXAS INSTRUMENTS",BLACK,GREEN,5,4);
  lcd_display_string( "XIDIAN UNIVERSITY",BLACK,GREEN,5,6);
  drawHeart();
  UARTInit();		//初始化RS232接口
  initADC();
  initDAC();
  initDCMotor();
  stepMotorInit();
  initLcdSeg();
  _EINT();
  P4DIR |= 0xe0;                              //LED
  P4OUT &= 0x1f;
  LCDSEG_DisplayNumber(123456,0);

  audio();
  startMotor(0,1,40*30,300,20000);
  while(motor0.running){
   send(0xf2,1,(int16_t)motor0.step_speed);
   delay_ms(20);
  }
  
  
  while(1)
  {
    P4OUT^=0x10<<i;
    LCDSEG_SetSpecSymbol(i);    
    val = adc0; //获取ADC转换结果
    val = val * PWM_COUNT * 2 / 4095 - PWM_COUNT; //将ADC转换结果从0~4095转换到-399~399
    PWM_SetOutput(val); //修改PWM输出电压
    send(0xf1,2,(int16_t)val,(int16_t)getRPM()*4/32);       
    if(++i>3){
      i=1;
      LCDSEG_DisplayNumber(getRPM(), 0); //更新转速显示
      LCDSEG_SetSpecSymbol(0);              //电池电量符号   
     }
      _BIS_SR(LPM0_bits + GIE);
//      delay_ms(333);
  }
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  adc0 = ADC12MEM0; //保存转换结果
  ADC12CTL0 &= ~ADC12ENC; //重置ADC，准备下一次采样
  ADC12CTL0 |= ADC12ENC;
}
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	_BIC_SR_IRQ(LPM0_bits);
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_a0()
{
  
   refClockCount();
   T0_int();
}
#pragma vector=TIMER0_B1_VECTOR
__interrupt void timer0_b0()
{

   switch(TBIV)
   {
   case TB0IV_TB0IFG: 
     SPTA_IRQHandler(&motor0);
     break;
   default: break;
   }
}
#pragma vector=USCI_A1_VECTOR	//USCI中断服务函数
__interrupt void USCI_A1_ISR(void)
{
	switch(__even_in_range(UCA1IV,4))
	{
	case 0:break;			//无中断
	case 2:					//接收中断处理
		UARTSendChar(UCA1RXBUF);
		break;
	case 4:break;			//发送中断不处理
	default:break;			//其他情况无操作

	}
}