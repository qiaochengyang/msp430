#ifndef __DR_STEP_MOTOR_H
#define __DR_STEP_MOTOR_H
#include <stdint.h>
typedef struct{
    uint8_t dir;
    uint8_t running;
    uint8_t id;
    uint32_t step_move;
    uint32_t step_count;
    uint32_t step_middle;
    uint32_t speed_frac;
    uint32_t speed_accel;
    uint32_t step_frac;
    uint32_t step_speed;
    uint32_t step_spmax;
    uint8_t step_state;
    uint32_t current_pulse;
} MOTOR;
void step(int p);
void motorDirSet(uint8_t motor_id,uint8_t dir);
void motorClkSet(uint8_t morot_id, uint8_t level);
void stepMotorInit(void);
void SPTA_IRQHandler(MOTOR *pmotor);
void startMotor(uint8_t motor_id,uint8_t dir,int32_t degree,uint32_t step_spmax,uint32_t speed_accel);
extern MOTOR motor0;
#endif