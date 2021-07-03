#ifndef __ADC_H
#define __ADC_H	  
#include "HAL_device.h"

void ADC1_CHx_Init(u8 ADC1_CHl_x) ;
u16 ADC1_Read(u8 ADC1_CHl_x) ;
u16 ADC1_Read_Average(u8 ADC1_CHl_x,u8 times);
void Test_ADC1(void);
void adc_init(void);

extern float adc_left;
extern float adc_right;

#endif

