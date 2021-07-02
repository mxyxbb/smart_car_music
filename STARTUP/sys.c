#include "include.h"
/*us延时倍乘数*/		
static u8  fac_us=0; 
/*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/
static u16 fac_ms=0;
u8 rx_cnt = 0, tx_flag;
u32 SysTick_Count;
u32 gDlycnt;

/********************************************************************************************************
**函数信息 ：void delayms(u16 nms)    
**功能描述 ：延时nms
**输入参数 ：nms
**输出参数 ：
**    备注 ：SysTick->LOAD为24位寄存器,所以,最大延时为:nms<=0xffffff*8*1000/SYSCLK,72M条件下,nms<=1864 
********************************************************************************************************/
void delayms(u16 nms)
{
  u32 temp;
  /*时间加载(SysTick->LOAD为24bit)*/    
  SysTick->LOAD=(u32)nms*fac_ms;
  /*清空计数器*/    
  SysTick->VAL =0x00;	
  /*开始倒数*/    
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	                               
  do{
    temp=SysTick->CTRL;
  }
  /*等待时间到达*/
  while((temp&0x01)&&!(temp&(1<<16)));		                            
  /*关闭计数器*/
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	                           
  /*清空计数器*/    
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
**函数信息 ：void delay_init(void)                         
**功能描述 ：初始化延迟函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void mysys_init(void)
{
  InitSystick();
  RCC_ConfigInit();//开启ABCD口的时钟  
  /*选择外部时钟  HCLK/8*/
  SysTick_CLKSourceConfig(8);
  /*为系统时钟的1/8*/    
  fac_us=SystemCoreClock/8000000;				                               
  /*非OS下,代表每个ms需要的systick时钟数 */
  fac_ms=(u16)fac_us*1000;					                              
}
/*************************************************************************
*  函数名称：void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
*  功能说明：中断初始化
*  参数说明：//  @param      irqn              中断号，可以查看CH32V10x.h文件中的IRQn_Type枚举体定义
            //  @param      pre_prior         抢占优先级,值越小优先级越高
            //  @param      sub_prior         次优先级,值越小优先级越高
            //  @param      status            使能或者禁止
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：    nvic_init(EXTI0_IRQn,1, 0, ENABLE); //外部中断1使能,抢占优先级次高，次优先级最高。
*************************************************************************/
void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irqn;                        //中断号
    NVIC_InitStructure.NVIC_IRQChannelPriority=pre_prior;   //抢占优先级,值越小优先级越高    
    NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)status;                   //使能
    NVIC_Init(&NVIC_InitStructure);
}

