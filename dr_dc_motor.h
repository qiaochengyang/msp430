#ifndef __DR_DC_MOTOR_H_
#define __DR_DC_MOTOR_H_
#define PWM_FREQ     10000
#define PWM_COUNT (SMCLK_FREQ / PWM_FREQ - 1)
//��ʼ��ֱ�����ģ�飬������ض˿����á���ʱ�����ú�ת�ٲ�������
void initDCMotor();
void refClockCount(void);
//�趨PWM���ֵ���趨ֵӦ����-PWM_COUNT~+PWM_COUNT
void PWM_SetOutput(int val);

//���ݵ�ǰ��ת�ٲ����������ת��ֵ����λΪRPM
int getRPM();

#endif
