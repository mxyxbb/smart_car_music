#include "include.h"
#include "adc.h"

u32 rotate_speed = 200;
char txt[32];
void PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_StructInit;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/*************************************引脚配置******************************/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7,GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,GPIO_AF_1); 
	
	//added by maxueyang 2021-06-17 for beeper
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7,GPIO_AF_6); 
	
	
	//TIM3_CH1234
  GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	//added by maxueyang 2021-06-17 for beeper
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	
	/**************************************************************************/
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //开启定时器3时钟
	
	//added by maxueyang 2021-06-17 for beeper
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);  //开启定时器16时钟
	
	
	
	//初始化TIM
  TIM_StructInit.TIM_Period = 959;                        //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_StructInit.TIM_Prescaler = 6 ;                         //设置用来作为TIMx时钟频率除数的预分频值
  TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
  TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
  TIM_StructInit.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(((TIM_TypeDef *) TIM3_BASE), &TIM_StructInit);  //初始化TIM3的时基
	
	//added by maxueyang 2021-06-17 for beeper
	//初始化TIM
  TIM_StructInit.TIM_Period = 959;                        //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_StructInit.TIM_Prescaler = 6 ;                         //设置用来作为TIMx时钟频率除数的预分频值
  TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
  TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
  TIM_StructInit.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(((TIM_TypeDef *) TIM17_BASE), &TIM_StructInit);  //初始化TIM17的时基
	
	
	
	//初始化TIM1 PWM模式
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                       //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //比较输出使能
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0;																			//pwm初始化
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                //输出极性:TIM输出比较极性
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;               //输出极性:TIM输出比较极性
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure );                       //定时器通道1初始化
  TIM_OC2Init(TIM3, &TIM_OCInitStructure );
	TIM_OC3Init(TIM3, &TIM_OCInitStructure );
	TIM_OC4Init(TIM3, &TIM_OCInitStructure );
	
	TIM_OC1PreloadConfig(((TIM_TypeDef *) TIM3_BASE), TIM_OCPreload_Enable);               //定时器预装载配置        
  TIM_OC2PreloadConfig(((TIM_TypeDef *) TIM3_BASE), TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(((TIM_TypeDef *) TIM3_BASE), TIM_OCPreload_Enable);        
  TIM_OC4PreloadConfig(((TIM_TypeDef *) TIM3_BASE), TIM_OCPreload_Enable);        
  
  TIM_ARRPreloadConfig(((TIM_TypeDef *) TIM3_BASE), ENABLE );                                               
  TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIM3_BASE), ENABLE );
  TIM_Cmd(((TIM_TypeDef *) TIM3_BASE), ENABLE);   //定时器使能
	PWM_Set_Duty(0,0,0,0);//0-959
	
	//added by maxueyang 2021-06-17 for beeper
	//初始化TIM17 PWM模式
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                       //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //比较输出使能
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0;																			//pwm初始化
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                //输出极性:TIM输出比较极性
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;               //输出极性:TIM输出比较极性
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC1Init(TIM17, &TIM_OCInitStructure );                       //定时器通道1初始化
  TIM_OC1PreloadConfig(((TIM_TypeDef *) TIM17_BASE), TIM_OCPreload_Enable);               //定时器预装载配置        
  TIM_ARRPreloadConfig(((TIM_TypeDef *) TIM17_BASE), ENABLE );                                               
  TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIM17_BASE), ENABLE );
  TIM_Cmd(((TIM_TypeDef *) TIM17_BASE), ENABLE);   //定时器使能
}




void PWM_Set_Duty(u32 duty1,u32 duty2,u32 duty3,u32 duty4)
{
	((TIM_TypeDef *) TIM3_BASE)->CCR1=duty1;
	((TIM_TypeDef *) TIM3_BASE)->CCR2=duty2;	
	((TIM_TypeDef *) TIM3_BASE)->CCR3=duty3;	
	((TIM_TypeDef *) TIM3_BASE)->CCR4=duty4;
}

void motor_run(u32 left_pwm, u32 right_pwm)//直立车，面朝行进方向，leftpwm控制右轮，rightpwm控制左轮
{
	if(left_diretion==1)
	{
		((TIM_TypeDef *) TIM3_BASE)->CCR4=0;
	  ((TIM_TypeDef *) TIM3_BASE)->CCR2=left_pwm;	
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	else if(left_diretion==2)
	{
		((TIM_TypeDef *) TIM3_BASE)->CCR4=left_pwm;
	  ((TIM_TypeDef *) TIM3_BASE)->CCR2=0;
	}
	else
	{
		((TIM_TypeDef *) TIM3_BASE)->CCR4=0;
	  ((TIM_TypeDef *) TIM3_BASE)->CCR2=0;
	}
		
	if(right_diretion==1)
	{
		((TIM_TypeDef *) TIM3_BASE)->CCR3=0;	
		((TIM_TypeDef *) TIM3_BASE)->CCR1=right_pwm;//-5;
	}
	else if(right_diretion==2)
	{
		((TIM_TypeDef *) TIM3_BASE)->CCR3=right_pwm;//+5;	
		((TIM_TypeDef *) TIM3_BASE)->CCR1=0;
	}
	else
	{
		((TIM_TypeDef *) TIM3_BASE)->CCR3=0;	
		((TIM_TypeDef *) TIM3_BASE)->CCR1=0;
	}
}

void rotate_1()//右旋
{
	left_diretion=1;
	right_diretion=2;
	while(1)
	{
		motor_run(rotate_speed, rotate_speed);
		sprintf(txt, "adc1: %05d;", adc_left);
		TFTSPI_P8X16Str(3, 3, txt, u16PURPLE, u16BLACK);   // 字符串显示
		sprintf(txt, "adc2: %05d;", adc_right);
		TFTSPI_P8X16Str(3, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
		if((adc_left+adc_right)<=800) break;
	}
}

void rotate_2()//左旋
{
	left_diretion=2;
	right_diretion=1;
	while(1)
	{
		motor_run(rotate_speed, rotate_speed);
		sprintf(txt, "adc1: %05d;", adc_left);
		TFTSPI_P8X16Str(3, 3, txt, u16PURPLE, u16BLACK);   // 字符串显示
		sprintf(txt, "adc2: %05d;", adc_right);
		TFTSPI_P8X16Str(3, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
		if((adc_left+adc_right)<=800) break;
	}
}


