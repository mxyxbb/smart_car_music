#include "adc.h"
#include "CONTROLTASK/control_task.h"

float adc_normal=2000;
float adc_O=2500;
float adc_Y=1300;


int judgeOandY()
{
	int adc_sum;
	adc_sum=adc_left+adc_right;
	if(adc_sum/adc_normal>1.3)
		return HUANDAO;
	else if(adc_sum/adc_normal<0.7)
		return SANCHA;
	else
		return ZHIDAO;
}
