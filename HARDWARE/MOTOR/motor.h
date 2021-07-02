#ifndef __MOTOR_H
#define __MOTOR_H	  

#include "HAL_device.h"
#include "HAL_conf.h"

void PWM_Init(void);
void PWM_Set_Duty(u32 duty1,u32 duty2,u32 duty3,u32 duty4);
void motor_run(u32 left_speed, u32 right_speed);
void rotate_1(void);
void rotate_2(void);


extern u8 left_diretion;
extern u8 right_diretion;


#endif

