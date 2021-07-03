#ifndef __BALANCE_H
#define __BALANCE_H	
#include "MM32SPIN2xx_p.h"


int balance(void);
void set_pwm(int pwm1,int pwm3);
int velocity(int encoder_left,int encoder_right);
int turn(int encoder_left,int encoder_right);
uint8_t angle_is_normal(void);

extern char Flag_Stop;
extern float Balance_Kp,Balance_Kd,Velocity_Kp,Velocity_Ki;//ok parameter 2021/06/23-11:13
extern float maxueyang_speed;
extern volatile float Encoder_Integral;
extern int motor1,motor2;
extern float ZHONGZHI;//heavy block, debug block, and battary inside [MPU2]

#endif 




