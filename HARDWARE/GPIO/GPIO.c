#include "GPIO.h"
#include "HAL_device.h"
#include "HAL_conf.h"
#include "NIMING/data_transfer.h"

extern u8 if_car_move;
extern int left_error;

char if_key;


void GPIO_LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void GPIO_beep_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

}
void GPIO_KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  //key0对应C3，key1对应C2，key2对应c1
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//设置为上拉输入，当按键按下时，IO口读到低电平
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void LED_trun(void)
{
	static u8 IF_LED_B10_TURN = 0;
	static u8 IF_LED_C4_TURN = 0;
	IF_LED_B10_TURN++;
	IF_LED_C4_TURN++;
	if(IF_LED_B10_TURN >= 20)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		IF_LED_B10_TURN = 0;
	}
	else if(IF_LED_B10_TURN == 10)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	}
	
	if(IF_LED_C4_TURN >= 10)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_4);
		IF_LED_C4_TURN = 0;
	}
	else if(IF_LED_C4_TURN == 5)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	}
}
int if_key_zero(void)
{
	int i = 0;
	i = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3);
	if(i==1)	return 0;
	else return 1;
}
int if_key_one(void)
{
	int i = 0;
	i = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
	if(i==1)	return 0;
	else return 1;
}
int if_key_two(void)
{
	int i = 0;
	i = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
	if(i==1)	return 0;
	else return 1;
}
void beep_on_once(void)
{
	beep_ON();
	delayms(100);
	beep_OFF();
}
void beep_briefly_on(void)
{
	beep_ON();
	delayms(100);
	beep_OFF();
	delayms(100);
	beep_ON();
	delayms(100);
	beep_OFF();
	delayms(100);
	beep_ON();
	delayms(100);
	beep_OFF();
}
void beep_ON(void)
{
	GPIO_SetBits(GPIOD,GPIO_Pin_6);
}
void beep_OFF(void)
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);
}
void led_toggle(void)
{
	static u8 i;
	i=!i;
	GPIO_WriteBit(GPIOC, GPIO_Pin_4, i);
}


void start_or_stop(void)
{
	int out_way_tft_show = 0;
	int key_stop_state = 0;//通过按键停车
	char txt[32];
	u16 vbat;
	while(1)
	{
		
		

		
		vbat=ADC1_Read(0);   
		sprintf(txt, "C1:%5d", vbat);  // *3.3/4095*3
    TFTSPI_P8X16Str(1, 1, txt, u16PURPLE, u16BLACK);   // 字符串显示
		
		vbat=ADC1_Read(6);   
		sprintf(txt, "C5:%5d", vbat);  // *3.3/4095*3
    TFTSPI_P8X16Str(1, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示
		
		
		

		
		
		
		
		
		
		if(if_key==1) //第一个按键按下，发车
		{
			beep_briefly_on();
			if_car_move = 1;
			key_stop_state = 0;//可以显示出轨停车
			TFTSPI_CLS(u16BLUE); 
			TFTSPI_P8X16Str(3, 1, "START", u16PURPLE, u16BLACK); 
			out_way_tft_show = 0;
		}
		if(if_key==2) //第二个按键按下，停车
		{
			beep_briefly_on();
			if_car_move = 0;
			key_stop_state = 1;//不可以显示出轨停车（按键停车优先级大于出轨检测停车）
			PWM_Set_Duty(0,0,0,0);//停车
			TFTSPI_CLS(u16BLUE); 
			TFTSPI_P8X16Str(3, 1, "STOP", u16PURPLE, u16BLACK); 
		}
		if((if_car_move == 2)&&(out_way_tft_show==0)&&(key_stop_state==0))//显示停车
		{
			//判断的三个参数的意义：
			//adc_value的值明显小于在线上，TFT屏只显示一次，是不是按键触发了停车（按键停车优先级大于出轨停车）
			TFTSPI_CLS(u16BLUE); 
			TFTSPI_P8X16Str(3, 1, "OUTWAY_STOP", u16PURPLE, u16BLACK); 
			out_way_tft_show = 1;
			key_stop_state = 1;
		}
		if_key=0;
	
		
		
		
		//send_speed_and_pwm();
		//send_error();
		



	}
}



