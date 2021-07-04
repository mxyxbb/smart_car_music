#include "include.h"
#include "buzzerDriver.h"
#include "shell_port.h"
#include "angle.h"
#include "mymath.h"
#include "multi_button/multi_button.h"
#include "BALANCE/balance.h"
#include "speedcontrol.h"
#include "hellodata/hellodata.h"
#include "adc.h"
#include "CONTROLTASK/control_task.h"

const u32 TIMERx[] = {TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM8_BASE, TIM14_BASE, TIM16_BASE, TIM17_BASE};

u8 if_car_move = 0;
char str_timer[255];

int pwm1,pwm2,pwm3;
u16 time0_timer = 0;
u8 time1_timer_flag;
#define TASK0_INIT_TIME 200 //200*10=2000ms

u16 time1_timer = 0; //for stand up
#define TASK1_STAND_TIME 50 //50*10=500ms

u16 sancha_timer = 0;
#define TASK2_SANCHA_TIME 50 //50*10=500ms

u16 begin_cooler_timer =0;
#define TASK3_COOLER_TIME 10 //10*10=100ms


u8 buzzer_flag;//���Ʒ�������һ�±�־λ
u8 if_music1,if_music2;



void TIM14_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������?:TIM �ж�Դ
  {
    TIM_ClearITPendingBit(TIM14, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
    //�û�����    
		//!!!!!!!!!!!!!!!!!!!!!
		//@@@@@@@@@@@@@@@@@@@@@
		//#####################
		//$$$$$$$$$$$$$$$$$$$$$
	}  
}

void TIM16_IRQHandler(void)
{
	static int adc_left_last; 
	static u32 adc_left_filt_flag = 0;
	static int adc_right_last; 
	static u32 adc_right_filt_flag = 0;
	
	static u8 buzzer_on_flag,buzzer_tick;
	
  if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������?:TIM �ж�Դ
  {
    TIM_ClearITPendingBit(TIM16, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
    //�û����� 
		
		
		button_ticks();	//�����¼�����
		music5_ticks();	
		
		//��Դ��������������һ�£�buzzer_flag��1
		if(buzzer_flag)
			buzzer_tick++;
		else
			buzzer_tick=0;
		if(buzzer_flag==1&&buzzer_on_flag==0)
		{
			beep_ON();
			buzzer_on_flag=1;
			buzzer_flag=1;
		}
		else if(buzzer_tick>5)
		{
			beep_OFF();
			buzzer_on_flag=0;
			buzzer_flag=0;
		}
		


		
    left_encoder_puslse = (short)TIM2 -> CNT;  TIM2 -> CNT=0;//��������
    right_encoder_puslse = (short)TIM1 -> CNT;  TIM1 -> CNT=0;//��������
		
		left_encoder_puslse = -left_encoder_puslse;
		now_speed_left = left_encoder_puslse*0.04491655*5;
		now_speed_right = right_encoder_puslse*0.04491655*5;
		
		
		__ExecuteOnce(adc_left_last=ADC1_Read(0));
		__ExecuteOnce(adc_right_last=ADC1_Read(6));
		/*-----------------��·ADC�˲�-----------------*/
		adc_left=ADC1_Read(0);
		adc_left=ADC1_Read(0);
		adc_left=ADC1_Read(0);
		adc_left=ADC1_Read(0);
		
//		if((float_Abs((float)(adc_left)/(float)(adc_left_last))>1.3||(float_Abs((float)(adc_left)/(float)(adc_left_last))<0.8))&&(adc_left_filt_flag>5)) 
//		{
//			adc_left = adc_left_last;
//			adc_left_filt_flag = 0;
//		}
//		adc_left_last = adc_left;
//		adc_left_filt_flag++;
		
		
		adc_right=ADC1_Read(6);
//		if((float_Abs((float)(adc_right)/(float)(adc_right_last))>1.3||(float_Abs((float)(adc_right)/(float)(adc_right_last))<0.8))&&(adc_right_filt_flag>5)) 
//		{
//			adc_right = adc_right_last;
//			adc_right_filt_flag = 0;
//		}
//		adc_right_last = adc_right;
//		adc_right_filt_flag++;
		/*-------------------------------------*/
		
		
//		adc_left_vertical=ADC1_Read(0);
//		adc_left_level=ADC1_Read(1);

//		adc_right_level=ADC1_Read(6); 
//		adc_right_vertical=ADC1_Read(7);
		
//		adc_second_left=ADC1_Read(8);
//		adc_second_right=ADC1_Read(9);

		
//		switch(adc_value_processing())
//		{
//			case 0:
//				if_car_move = 2;//���켱ͣ
//				break;
//			case 1:
//			case 2:
//			case 3:
//			case 4:
//			case 5:
//			case 6:
//			case 7:  
//			default:
//				break;	
//		}
		
		
		///////////////////////////////////////////
		
		cacl_error_1();
		angle_hanlder();
		if(angle<-108||angle>1) // lie down check, avoid rushing(not used)
		{
			Flag_Stop=1; // flag to indicate the car is at the state -- [stop], can be referrenced by other function.
			pwm1=0;
			pwm2=0;
			pwm3=0;
		}
		else // normal task
		{
			if(time0_timer>TASK0_INIT_TIME)
			{
				Flag_Stop=0; // flag to indicate the car is at the state -- [running], can be referrenced by other function.
				pwm1=balance(); // [balance loop]
				if(time1_timer_flag && angle_is_normal())// the car has stood up, [turn on] the [velocity loop].
				{
					pwm2=velocity(left_encoder_puslse,right_encoder_puslse); // [velocity loop]
					
					
					if(begin_cooler_timer>TASK3_COOLER_TIME)
					{
						//judge O or Y here
						switch (judgeOandY())
						{
							case HUANDAO:
								pwm3=turn(left_encoder_puslse,right_encoder_puslse);
								break;
							case SANCHA:
								if(sancha_timer<TASK2_SANCHA_TIME)
								{
									//musicPlay();
									music5_play_flag=1;
									sancha_timer++;
									pwm3=50;
								}
								else
									pwm3=turn(left_encoder_puslse,right_encoder_puslse);
								break;
							default:
								pwm3=turn(left_encoder_puslse,right_encoder_puslse);
								break;
						
							
						}
					}
					else
					{
						pwm3=0;
						begin_cooler_timer++;
					}
					
					
				}
				else// the car has not stood up, [turn off] the [velocity loop], avoid flying.
				{
					if(time1_timer>TASK1_STAND_TIME)
						time1_timer_flag=1;
					else
						time1_timer++;
					pwm2=0;
				}
				set_pwm(pwm1+pwm2,pwm3);
			}
			else
			{
				time0_timer++;
				set_pwm(0,0);
			}
		}
		
/*		
		if(if_car_move==1)
		{
			//cacl_error_1();//�ñ�ֵ��
			sum_speed();
			update_pwm();
			motor_run(left_pwm,right_pwm);
		}
		else if(if_car_move==0)//�ջ���0
		{
			motor_run(0,0);
		}
		else if(if_car_move==2)
		{
			sum_speed();
			//cacl_error_1();
			sum_left_speed = 0;
			sum_right_speed = 0;
			update_pwm();
			motor_run(left_pwm,right_pwm);
		}
		else 
		{
			motor_run(0,0);
		}
*/

//    music4Play();

		if(if_car_move==1)
		{
			music4Play();
		}
		else
		{
			if(if_music1==1)
			{
				music4Play();
			}
			else if(if_music2==1)
			{
				musicPlay();
			}
			else if(if_music1==0&&if_music2==0)
			{
				//TIM17->CCR1=0;
			}
		}
		

		
		
		
		LED_trun();
//		
	}  
}
void TIM17_IRQHandler(void)
{
	
  if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������?:TIM �ж�Դ
  {
    TIM_ClearITPendingBit(TIM17, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
    
		
		
	}  
}
//��ʱ��1�жϷ������?
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{ 	
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������?:TIM �ж�Դ
  {
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
    //�û�����    
    
  }  
}

/*************************************************************************
*  �������ƣ�void TIMER_InitConfig(TIMER_Name_t timern, u16 ms)
*  ����˵�������PWM��ʼ��
*  ����˵����
//  @param      timern      ��ʱ��ͨ��
//  @param      ms          ��ʱ����pch,PWMͨ������Ӧ�ĵĶ�ʱ�����ܽ�

*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��TIMER_InitConfig(TIMER_1, 5); ʹ�ö�ʱ��1��Ϊ5msһ�ε������ж�
*************************************************************************/
void TIMER_InitConfig(TIMER_Name_t timern, u16 ms)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

         if(TIMER_1 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);                //ʱ��ʹ��
    else if(TIMER_2 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(TIMER_3 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(TIMER_8 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    else if(TIMER_14 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM14, ENABLE);
    else if(TIMER_16 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);    
    else if(TIMER_17 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
    
    //��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = ms<<1;                               //�����Զ���װ�ؼĴ�������
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/2000);           //����Ϊ2Khz����һ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //����ʱ��Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                        //�ظ�����������Ϊ0
    TIM_TimeBaseInit((TIM_TypeDef *)TIMERx[timern], &TIM_TimeBaseStructure);//��ʼ��TIMxʱ��

    TIM_ITConfig((TIM_TypeDef *)TIMERx[timern],TIM_IT_Update,ENABLE );      //ʹ��TIM�ж�

    //�ж����ȼ�����
         if(TIMER_1 == timern)   nvic_init(TIM1_BRK_UP_TRG_COM_IRQn, 0, 2, ENABLE);
    else if(TIMER_2 == timern)   nvic_init(TIM2_IRQn, 0, 2, ENABLE);
    else if(TIMER_3 == timern)   nvic_init(TIM3_IRQn, 0, 2, ENABLE);
    else if(TIMER_8 == timern)   nvic_init(TIM8_BRK_UP_TRG_COM_IRQn, 0, 2, ENABLE);
    else if(TIMER_14 == timern)   nvic_init(TIM14_IRQn, 0, 2, ENABLE);
    else if(TIMER_16 == timern)   nvic_init(TIM16_IRQn, 0, 2, ENABLE);
    else if(TIMER_17 == timern)   nvic_init(TIM17_IRQn, 0, 2, ENABLE);
    TIM_Cmd((TIM_TypeDef *)TIMERx[timern], ENABLE);  //ʹ��TIMx

}
