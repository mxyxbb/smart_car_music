/*
MM32SPIN27PS
**ADC1:               
**ADC1CH0 : A0  C1    
**ADC1CH1 : A1	C2
**ADC1CH2 : A2          
**ADC1CH3 : A3          
**ADC1CH4 : A4  C3         
**ADC1CH5 : A5	C4
**ADC1CH6 : A6	C5
**ADC1CH7 : A7	C6
**ADC1CH8 : B0
**ADC1CH9 : B1
**ADC1CH10: B2
**ADC1CH11: B10
**ADC1CH12: C11
**ADC2:
**ADC2CH0:  A10
**ADC2CH1:  B15
**ADC2CH2:  B14
**ADC2CH3:  B13
**ADC2CH4:  A4
**ADC2CH5:  A5
**ADC2CH6:  A6
**ADC2CH7:  A7
**ADC2CH8:  B0
**ADC2CH9:  B1
**ADC2CH10: B2
**ADC2CH11: B10
*/


#include "include.h"

int adc_left = 0;
int adc_right = 0;
int adc_left_level = 0;
int adc_left_vertical = 0;
int adc_right_level = 0;
int adc_right_vertical = 0;
int adc_second_left = 0;
int adc_second_right = 0;


void ADC1_CHx_Init(u8 ADC1_CHl_x) //入口参数为0-11
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	ADC_InitTypeDef  ADC_InitStructure;	
//////////////////////使能时钟////////////////////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);//GPIOB时钟使能
/////////////////////////////////////////////////////////////////////

//////////////////////引脚初始化//////////////////////////////////////
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	switch (ADC1_CHl_x)
  {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:    
    GPIO_InitStructure.GPIO_Pin  =  ADC1_CHl_x;
    GPIO_Init(GPIOA, &GPIO_InitStructure);   
    break;
  case 8:   
  case 9:    
  case 10:
    GPIO_InitStructure.GPIO_Pin  =  ADC1_CHl_x-8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
    break;
  case 11:
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
    break;    
  default:
    break;      
  }  
/////////////////////////////////////////////////////////////////////
	
//////////////////////ADC初始化//////////////////////////////////////
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//分辨率 ADC resolution select 12bit 
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;//时钟分频
  ADC_InitStructure.ADC_Mode = ADC_Mode_Continuous_Scan;//连续转换模式
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_Init(ADC1, &ADC_InitStructure);  
/////////////////////////////////////////////////////////////////////
  ADC_RegularChannelConfig(ADC1, ADC1_CHl_x,0, ADC_SampleTime_1_5Cycles);//采样周期
	ADC_Cmd(ADC1, ENABLE);  //使能ADC
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u16 ADC1_Read(u8 ADC1_CHl_x) 
{ 
  while(ADC1_CHl_x!=(((ADC1->ADDATA) >>16) & 0xf)); //找到对应的通道 
  while(!(ADC1->ADSTA&ADC_FLAG_EOC));                  //等待转换完成  
  return (ADC1->ADDATA&0xfff);                         //读取数据
}

u16 ADC1_Read_Average(u8 ADC1_CHl_x,u8 times)
{
  u32 temp_val=0;
  u8 t;
  u8 delay;
  for(t=0;t<times;t++)
  {
    temp_val+=ADC1_Read(ADC1_CHl_x);
    for(delay=0;delay<100;delay++);
  }
  return temp_val/times;
} 

void adc_init()
{
	ADC1_CHx_Init(ADC_Channel_0);
	ADC1_CHx_Init(ADC_Channel_1);
  ADC1_CHx_Init(ADC_Channel_4);
  ADC1_CHx_Init(ADC_Channel_5);
	ADC1_CHx_Init(ADC_Channel_6);
  ADC1_CHx_Init(ADC_Channel_7);
	ADC1_CHx_Init(ADC_Channel_8);
  ADC1_CHx_Init(ADC_Channel_9);

}
void Test_ADC1(void)
{
  u16 vbat;
  char txt[16];
  adc_init();
  
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕	
  while(1)
  {
    vbat=ADC1_Read(0);   
		sprintf(txt, "C1:%5d", vbat);  // *3.3/4095*3
    TFTSPI_P8X16Str(1, 1, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
//    sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3

		vbat=ADC1_Read(1);   
		sprintf(txt, "C2:%5d", vbat);  // *3.3/4095*3
    TFTSPI_P8X16Str(1, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
//   sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3
//    TFTSPI_P8X16Str(1, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
		
		vbat=ADC1_Read(4);   
		sprintf(txt, "C3:%5d", vbat);  // *3.3/4095*3
    TFTSPI_P8X16Str(1, 3, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
//    sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3
//    TFTSPI_P8X16Str(1, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
		
		vbat=ADC1_Read(5);   
		sprintf(txt, "C4:%5d", vbat);  // *3.3/4095*3
    TFTSPI_P8X16Str(1, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
//    sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3
//    TFTSPI_P8X16Str(1, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		
		vbat=ADC1_Read(6);   
		sprintf(txt, "C5:%5d", vbat);  // *3.3/4095*3
    TFTSPI_P8X16Str(1, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
////    sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3
////    TFTSPI_P8X16Str(1, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		
//		vbat=ADC1_Read(7);   
//		sprintf(txt, "C6:%5d", vbat);  // *3.3/4095*3
//    TFTSPI_P8X16Str(1, 6, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
////    sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3
////    TFTSPI_P8X16Str(1, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
//	
//		vbat=ADC1_Read(8);   
//		sprintf(txt, "C7:%5d", vbat);  // *3.3/4095*3
//    TFTSPI_P8X16Str(1, 7, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
//		
//		vbat=ADC1_Read(9);   
//		sprintf(txt, "C8:%5d", vbat);  // *3.3/4095*3
//    TFTSPI_P8X16Str(1, 8, txt, u16PURPLE, u16BLACK);   // 字符串显示
//		vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
  }
}

