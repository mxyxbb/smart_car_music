#include "include.h"
/*us��ʱ������*/		
static u8  fac_us=0; 
/*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/
static u16 fac_ms=0;
u8 rx_cnt = 0, tx_flag;
u32 SysTick_Count;
u32 gDlycnt;

/********************************************************************************************************
**������Ϣ ��void delayms(u16 nms)    
**�������� ����ʱnms
**������� ��nms
**������� ��
**    ��ע ��SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:nms<=0xffffff*8*1000/SYSCLK,72M������,nms<=1864 
********************************************************************************************************/
void delayms(u16 nms)
{
  u32 temp;
  /*ʱ�����(SysTick->LOADΪ24bit)*/    
  SysTick->LOAD=(u32)nms*fac_ms;
  /*��ռ�����*/    
  SysTick->VAL =0x00;	
  /*��ʼ����*/    
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	                               
  do{
    temp=SysTick->CTRL;
  }
  /*�ȴ�ʱ�䵽��*/
  while((temp&0x01)&&!(temp&(1<<16)));		                            
  /*�رռ�����*/
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	                           
  /*��ռ�����*/    
  SysTick->VAL =0X00;       					                               
}

/*****************************************************************************
* @name   : SysTick_Handler
* @brief  : SysTick interrupt handle function
* @param  : void
* @retval : void
*******************************************************************************/
void SysTick_Handler()
{
    if(SysTick_Count++ > 500)
    {
        SysTick_Count = 0;
    }
    if(gDlycnt > 0) gDlycnt --;
}
/*******************************************************************************
* @name   : RCC_ConfigInit()
* @brief  : RCC configure
* @param  : void
* @retval : void
*******************************************************************************/
void RCC_ConfigInit(void)
{  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
}
/*******************************************************************************
* @name   : InitSystick
* @brief  : Init Systick
* @param  : void
* @retval : void
*******************************************************************************/
void InitSystick()
{
  SysTick_Config(SystemCoreClock / 1000);
  NVIC_SetPriority(SysTick_IRQn, 0x00);
}


/********************************************************************************************************
**������Ϣ ��void delay_init(void)                         
**�������� ����ʼ���ӳٺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void mysys_init(void)
{
  InitSystick();
  RCC_ConfigInit();//����ABCD�ڵ�ʱ��  
  /*ѡ���ⲿʱ��  HCLK/8*/
  SysTick_CLKSourceConfig(8);
  /*Ϊϵͳʱ�ӵ�1/8*/    
  fac_us=SystemCoreClock/8000000;				                               
  /*��OS��,����ÿ��ms��Ҫ��systickʱ���� */
  fac_ms=(u16)fac_us*1000;					                              
}
/*************************************************************************
*  �������ƣ�void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
*  ����˵�����жϳ�ʼ��
*  ����˵����//  @param      irqn              �жϺţ����Բ鿴CH32V10x.h�ļ��е�IRQn_Typeö���嶨��
            //  @param      pre_prior         ��ռ���ȼ�,ֵԽС���ȼ�Խ��
            //  @param      sub_prior         �����ȼ�,ֵԽС���ȼ�Խ��
            //  @param      status            ʹ�ܻ��߽�ֹ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��    nvic_init(EXTI0_IRQn,1, 0, ENABLE); //�ⲿ�ж�1ʹ��,��ռ���ȼ��θߣ������ȼ���ߡ�
*************************************************************************/
void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irqn;                        //�жϺ�
    NVIC_InitStructure.NVIC_IRQChannelPriority=pre_prior;   //��ռ���ȼ�,ֵԽС���ȼ�Խ��    
    NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)status;                   //ʹ��
    NVIC_Init(&NVIC_InitStructure);
}

