#ifndef __TIMER_H_
#define __TIMER_H_
#include "include.h"


extern int left_encoder_puslse ;
extern int right_encoder_puslse ;
extern int sum_left_speed;
extern int sum_right_speed;
extern int adc_left_level;
extern int adc_left_vertical;
extern int adc_right_level;
extern int adc_right_vertical;
extern int adc_second_left;
extern int adc_second_right;
extern int pwm1,pwm2,pwm3;
extern u8 buzzer_flag;
extern u8 if_music1,if_music2;


typedef enum    //TIMER可用的管脚
{
    TIMER1_CHA_A8,
    TIMER1_CHB_A9,
    TIMER2_CHA_A0,
    TIMER2_CHB_A1,
    TIMER2_CHA_A15,              //完全映射
    TIMER2_CHB_B3,               //完全映射

    TIMER3_CHA_A6,
    TIMER3_CHB_A7,

    TIMER3_CHA_C6,               //完全映射
    TIMER3_CHB_C7,               //完全映射
    TIMER3_CHA_B4,               //部分映射
    TIMER3_CHB_B5,               //部分映射

    TIMER8_CHA_B6,
    TIMER8_CHB_B7,
    
    TIMER14_CHA_B6,
    TIMER14_CHB_B7,
    
    TIMER16_CHA_B6,
    TIMER16_CHB_B7,
    
    TIMER17_CHA_B6,
    TIMER17_CHB_B7,

}TIMER_PIN_Name_t;

typedef enum
{
    TIMER_1,
    TIMER_2,
    TIMER_3,
    TIMER_8,
    TIMER_14,
    TIMER_16,
    TIMER_17,
}TIMER_Name_t;

extern const u32 TIMERx[] ;
void TIM1_Int_Init(u16 x100us);
void TIMER_InitConfig(TIMER_Name_t timern, u16 ms);
#endif
