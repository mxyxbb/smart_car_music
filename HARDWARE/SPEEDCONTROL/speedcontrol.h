#ifndef __SPEEDCONTROL_H_
#define __SPEEDCONTROL_H_
#include "include.h"


int Abs(int i);
float float_Abs(float i);
void cacl_error(void);
void sum_speed(void);
void update_pwm(void);
void cacl_error_1(void);
float get_error_by_mean_filtering(float error);

extern int left_pwm;
extern int right_pwm;
extern int left_error;
extern int right_error;
extern double error;

#endif

