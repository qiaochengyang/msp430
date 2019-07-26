#include "msp430f6638.h"
#include "dr_step_motor.h"
#include <assert.h>
#include "clock.h"

#define IDLE 0
#define ACCELERATING 1
#define AT_MAX 2
#define DECELERATING 3

const uint32_t MAX_ACC_STEP = 123456;
const uint32_t ACC_SPEED = 1000;
const uint32_t START_SPEED = 50;

MOTOR motor0;
#define M0_CLOCKWISE 0

void motorDirSet(uint8_t motor_id,uint8_t dir)
 {
     
 }
 void motorClkSet(uint8_t motor_id, uint8_t level)
 {
     static uint8_t p=1;
     volatile MOTOR *pmotor = 0;
     switch(motor_id){
        case 0:
            pmotor=&motor0;
            break;   
    }
    if(pmotor->dir==1)
    {
        step(p++);
        if(p>4) p = 1;
    }
    else 
    {
        step(p--);
        if(p<1) p = 4;
    }
    
 }
void startMotor(uint8_t motor_id,uint8_t dir,int32_t degree,uint32_t step_spmax,uint32_t speed_accel)
{
    volatile MOTOR *pmotor = 0;
    if(degree<=0)
        return;
    motorDirSet(motor_id,dir);
    switch(motor_id){
        case 0:
            pmotor=&motor0;
            break;   
    }
    pmotor->dir = dir;
    pmotor->running = 1;
    pmotor->step_move = degree;
    pmotor->step_state = ACCELERATING;
    if((step_spmax==0)||(speed_accel==0)){
        pmotor->step_spmax = MAX_ACC_STEP;
        pmotor->speed_accel = ACC_SPEED;
    }
    else{
        pmotor->step_spmax = step_spmax;
        pmotor-> speed_accel = speed_accel;
    }
    pmotor->step_frac = 0;
    pmotor->speed_frac = 0;
    pmotor->step_speed = START_SPEED;
    pmotor->step_count = 0;
}
void SPTA_IRQHandler(MOTOR *pmotor)
{
    uint32_t carry=0;
    if( pmotor->running){
    pmotor->step_frac += pmotor->step_speed;
    carry = pmotor->step_frac>>17;
    pmotor->step_frac -= carry<<17;
    if(carry!=0){
        assert(carry==1);
        pmotor->step_count += 1;
        //cout << motor0.step_count <<","<< motor0.step_speed<<","<< motor0.step_state+64<< endl;
        motorClkSet(0,1);
        if(pmotor->step_count==pmotor->step_move){
                pmotor->step_state = IDLE;
                pmotor->running = 0;
        }
        else if((pmotor->step_move - pmotor->step_count <= pmotor->step_spmax)
            &&(pmotor->step_count >= (pmotor->step_move>>1))){
            pmotor->step_state = DECELERATING;
        }
        else if(pmotor->step_count>=pmotor->step_spmax){
            pmotor->step_state = AT_MAX;
        }
    }

    switch(pmotor->step_state){
        case ACCELERATING:
            pmotor->speed_frac += pmotor->speed_accel;
            carry = pmotor-> speed_frac>>17;
            pmotor->speed_frac -= (carry<<17);
            if(carry!=0){
                pmotor->step_speed += carry;
            }
            break;
        case AT_MAX:
            break;
        case DECELERATING:
            pmotor->speed_frac += pmotor->speed_accel;
            carry = pmotor->speed_frac>>17;
            pmotor->speed_frac -= (carry<<17);
            if(carry&&(pmotor->step_speed>carry)&&(pmotor->step_speed>START_SPEED)){
                pmotor->step_speed -= carry;
            }
            break;
        
    }
   }
}
void stepMotorInit(void)
{
  TB0CTL = TBSSEL__SMCLK + MC__UP + TBIE + TBCLR;
  TB0CCR0 = 399; //399

	__bis_SR_register(GIE);
  P1DIR |= BIT2 + BIT4;
	P2DIR |= BIT2 + BIT3;
	P7DIR |= BIT4;
	P7OUT |= BIT4;
}

void step(int p)
{
  switch(p)
  {
  case 0:
    {
      P7OUT &=~ BIT4;
      P1OUT &=~ BIT2;
      P2OUT &=~ BIT2;
      P1OUT &=~ BIT4;
      P2OUT &=~ BIT3;
      break;
    }
  case 1:
    {
      P7OUT |= BIT4;
      P1OUT |= BIT2;
      P2OUT &=~ BIT2;
      P1OUT &=~ BIT4;
      P2OUT &=~ BIT3;
      break;
    }
  case 3:
    {
      P7OUT |= BIT4;
      P1OUT &=~ BIT2;
      P2OUT |= BIT2;
      P1OUT &=~ BIT4;
      P2OUT &=~ BIT3;
      break;
    }
  case 2:
    {
      P7OUT |= BIT4;
      P1OUT &=~ BIT2;
      P2OUT &=~ BIT2;
      P1OUT |= BIT4;
      P2OUT &=~ BIT3;
      break;
    }
  case 4:
    {
      P7OUT |= BIT4;
      P1OUT &=~ BIT2;
      P2OUT &=~ BIT2;
      P1OUT &=~ BIT4;
      P2OUT |= BIT3;
      break;
    }
  default:
      break;
  }
}