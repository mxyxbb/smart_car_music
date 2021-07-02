#include "include.h"
#include "encoder.h"
#include "HAL_device.h"
#include "HAL_conf.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��Ϊ��ʱ����16λ�ġ�


int i = 0;








//������1��PB7��PB7
//������2��PA15��PB3
void encoder_init()
{    
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_6);//�ܽŸ���AF6
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_7);//�ܽŸ���AF7
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//PB6,7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���룬����
  GPIO_Init( GPIOB, &GPIO_InitStructure);	        
  
  TIM1->DIER|=1<<0;        // ��������ж�				
  TIM1->DIER|=1<<6;        // �������ж�	
  TIM1->PSC = 0x0;         // Ԥ��Ƶ��
  TIM1->ARR = ENCODER_TIM_PERIOD;//�趨�������Զ���װֵ 
  TIM1->CR1 &=~(3<<8);     // ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM1->CR1 &=~(3<<5);     // ѡ�����ģʽ:���ض���ģʽ		
  TIM1->CCMR1 |= 1<<0;     // CC1S='01' IC1FP1ӳ�䵽TI1
  TIM1->CCMR1 |= 1<<8;     // CC2S='01' IC2FP2ӳ�䵽TI2
  TIM1->CCER &= ~(1<<1);   // CC1P='0'	 IC1FP1�����࣬IC1FP1=TI1
  TIM1->CCER &= ~(1<<5);   // CC2P='0'	 IC2FP2�����࣬IC2FP2=TI2
  TIM1->CCMR1 |= 3<<4;     // IC1F='1000' ���벶��1�˲���
  TIM1->SMCR |= 3<<0;	   // SMS='011' ���е�������������غ��½�����Ч
  TIM1->CR1 |= 0x01;       // CEN=1��ʹ�ܶ�ʱ��
  
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_2);//�ܽŸ���AF2
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_2); //�ܽŸ���AF2
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //PA15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���룬����
  GPIO_Init( GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //PB3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���룬����
  GPIO_Init( GPIOB, &GPIO_InitStructure);
  
  //GPIOA->CRL&=0XFFFFFF00;// PA15 PB3
  //GPIOA->CRL|=0X00000044;// ��������
  TIM2->DIER|=1<<0;        // ��������ж�				
  TIM2->DIER|=1<<6;        // �������ж�	
  TIM2->PSC = 0x0;         // Ԥ��Ƶ��
  TIM2->ARR = ENCODER_TIM_PERIOD;//�趨�������Զ���װֵ 
  TIM2->CR1 &=~(3<<8);     // ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM2->CR1 &=~(3<<5);     // ѡ�����ģʽ:���ض���ģʽ		
  TIM2->CCMR1 |= 1<<0;     // CC1S='01' IC1FP1ӳ�䵽TI1
  TIM2->CCMR1 |= 1<<8;     // CC2S='01' IC2FP2ӳ�䵽TI2
  TIM2->CCER &= ~(1<<1);   // CC1P='0'	 IC1FP1�����࣬IC1FP1=TI1
  TIM2->CCER &= ~(1<<5);   // CC2P='0'	 IC2FP2�����࣬IC2FP2=TI2
  TIM2->CCMR1 |= 3<<4;     // IC1F='1000' ���벶��1�˲���
  TIM2->SMCR |= 3<<0;	   // SMS='011' ���е�������������غ��½�����Ч
  TIM2->CR1 |= 0x01;       // CEN=1��ʹ�ܶ�ʱ��
	
}

int read_encoder(u8 TIMX)
{
  int encoder_puslse;    
  switch(TIMX)
  {
  case 1:  encoder_puslse= (short)TIM1 -> CNT;  TIM1 -> CNT=0;break;//��������
  case 2:  encoder_puslse= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;//��������
  default:  encoder_puslse=0;
  }
  return encoder_puslse;
}


void test_encoder_lcd(void)
{
  char txt[32];
  
  TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ
  TFTSPI_P8X16Str(3, 2,"encoder test", u16PURPLE, u16BLACK); // �ַ�����ʾ
  encoder_init();
  while (1)
  {
    /* ��ȡ������ֵ */
		//���ж��ж�ȡ��������ֵ
		
    sprintf(txt, "Enc1: %05d;", left_encoder_puslse);
    TFTSPI_P8X16Str(3, 4, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    sprintf(txt, "Enc2: %05d;", right_encoder_puslse);
    TFTSPI_P8X16Str(3, 5, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
		sprintf(txt, "pwm1: %05d;", left_pwm);
    TFTSPI_P8X16Str(3, 6, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    sprintf(txt, "pwm2: %05d;", right_pwm);
    TFTSPI_P8X16Str(3, 7, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
  }
}





