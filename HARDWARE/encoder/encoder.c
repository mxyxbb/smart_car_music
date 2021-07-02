#include "include.h"
#include "encoder.h"
#include "HAL_device.h"
#include "HAL_conf.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为定时器是16位的。


int i = 0;








//编码器1：PB7、PB7
//编码器2：PA15、PB3
void encoder_init()
{    
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_6);//管脚复用AF6
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_7);//管脚复用AF7
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//PB6,7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//输入，悬空
  GPIO_Init( GPIOB, &GPIO_InitStructure);	        
  
  TIM1->DIER|=1<<0;        // 允许更新中断				
  TIM1->DIER|=1<<6;        // 允许触发中断	
  TIM1->PSC = 0x0;         // 预分频器
  TIM1->ARR = ENCODER_TIM_PERIOD;//设定计数器自动重装值 
  TIM1->CR1 &=~(3<<8);     // 选择时钟分频：不分频
  TIM1->CR1 &=~(3<<5);     // 选择计数模式:边沿对齐模式		
  TIM1->CCMR1 |= 1<<0;     // CC1S='01' IC1FP1映射到TI1
  TIM1->CCMR1 |= 1<<8;     // CC2S='01' IC2FP2映射到TI2
  TIM1->CCER &= ~(1<<1);   // CC1P='0'	 IC1FP1不反相，IC1FP1=TI1
  TIM1->CCER &= ~(1<<5);   // CC2P='0'	 IC2FP2不反相，IC2FP2=TI2
  TIM1->CCMR1 |= 3<<4;     // IC1F='1000' 输入捕获1滤波器
  TIM1->SMCR |= 3<<0;	   // SMS='011' 所有的输入均在上升沿和下降沿有效
  TIM1->CR1 |= 0x01;       // CEN=1，使能定时器
  
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_2);//管脚复用AF2
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_2); //管脚复用AF2
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //PA15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//输入，悬空
  GPIO_Init( GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //PB3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//输入，悬空
  GPIO_Init( GPIOB, &GPIO_InitStructure);
  
  //GPIOA->CRL&=0XFFFFFF00;// PA15 PB3
  //GPIOA->CRL|=0X00000044;// 浮空输入
  TIM2->DIER|=1<<0;        // 允许更新中断				
  TIM2->DIER|=1<<6;        // 允许触发中断	
  TIM2->PSC = 0x0;         // 预分频器
  TIM2->ARR = ENCODER_TIM_PERIOD;//设定计数器自动重装值 
  TIM2->CR1 &=~(3<<8);     // 选择时钟分频：不分频
  TIM2->CR1 &=~(3<<5);     // 选择计数模式:边沿对齐模式		
  TIM2->CCMR1 |= 1<<0;     // CC1S='01' IC1FP1映射到TI1
  TIM2->CCMR1 |= 1<<8;     // CC2S='01' IC2FP2映射到TI2
  TIM2->CCER &= ~(1<<1);   // CC1P='0'	 IC1FP1不反相，IC1FP1=TI1
  TIM2->CCER &= ~(1<<5);   // CC2P='0'	 IC2FP2不反相，IC2FP2=TI2
  TIM2->CCMR1 |= 3<<4;     // IC1F='1000' 输入捕获1滤波器
  TIM2->SMCR |= 3<<0;	   // SMS='011' 所有的输入均在上升沿和下降沿有效
  TIM2->CR1 |= 0x01;       // CEN=1，使能定时器
	
}

int read_encoder(u8 TIMX)
{
  int encoder_puslse;    
  switch(TIMX)
  {
  case 1:  encoder_puslse= (short)TIM1 -> CNT;  TIM1 -> CNT=0;break;//编码器用
  case 2:  encoder_puslse= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;//编码器用
  default:  encoder_puslse=0;
  }
  return encoder_puslse;
}


void test_encoder_lcd(void)
{
  char txt[32];
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_P8X16Str(3, 2,"encoder test", u16PURPLE, u16BLACK); // 字符串显示
  encoder_init();
  while (1)
  {
    /* 获取编码器值 */
		//在中断中读取编码器的值
		
    sprintf(txt, "Enc1: %05d;", left_encoder_puslse);
    TFTSPI_P8X16Str(3, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf(txt, "Enc2: %05d;", right_encoder_puslse);
    TFTSPI_P8X16Str(3, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示
		sprintf(txt, "pwm1: %05d;", left_pwm);
    TFTSPI_P8X16Str(3, 6, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf(txt, "pwm2: %05d;", right_pwm);
    TFTSPI_P8X16Str(3, 7, txt, u16PURPLE, u16BLACK);   // 字符串显示
  }
}





