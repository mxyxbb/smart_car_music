#ifndef __BALANCE_H
#define __BALANCE_H	

int balance(void);
void set_pwm(int pwm1,int pwm3);
int velocity(int encoder_left,int encoder_right);
int turn(int encoder_left,int encoder_right);

extern char Flag_Stop;
extern float Balance_Kp,Balance_Kd,Velocity_Kp,Velocity_Ki;//ok parameter 2021/06/23-11:13
extern float maxueyang_speed;
extern float Encoder_Integral;
#endif 



