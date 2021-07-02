#include "include.h"

int line_position = 0;
extern int adc_left;
extern int adc_right;
extern int adc_left_level;
extern int adc_left_vertical;
extern int adc_right_level;
extern int adc_right_vertical;
extern double kp;
extern double kd;
extern float basic_speed;
extern float average_speed;
extern int if_car_move;


void where_is_the_line(void)
{
	char txt[32];
	line_position = (adc_left + adc_right)/2;
	if(adc_right<adc_left) line_position = -line_position;  //正数为右，负数为左侧
	
	sprintf(txt, "2: %05d;", line_position);
  TFTSPI_P8X16Str(3, 8, txt, u16PURPLE, u16BLACK);   // 字符串显示
	
}

int if_outway(void)
{	
	where_is_the_line();
	if((line_position>0)&&(line_position>750) )//线在右侧，而且大于1500的一半，认为小车出轨
		return 1;//返回1右侧出轨，返回0 =无出轨，返回-1左侧出轨
	else if((line_position>0)&&(line_position<750) )
		return 0;
	else if((line_position<0)&&(Abs(line_position)<750) )
		return 0;
	else if((line_position<0)&&(Abs(line_position)>750) )
		return -1;
}

int adc_value_processing(void)
{
	int adc_value_sum = 0;
	adc_value_sum = adc_left+adc_right;//+adc_left_level+adc_left_vertical+adc_right_level+adc_right_vertical;
	
	if(adc_value_sum<400) return 0;//没有检测到电磁线，停车


}

int error_processing(float error)
{
	basic_speed = average_speed;
	kp=7;
	kd=15;
	
	/*
	if(if_car_move==2)
	{
		kp=0;
		kd=0;
		basic_speed = 0;
		return 0;
	}
		
	if(error<15)
	{
		kp=7;
		kd=15;
		beep_OFF();
		basic_speed = average_speed + 0*(15-error);
	}
	else if(error < 150)
	{
		kp=12;
		kd=10;
		beep_ON();
		basic_speed = average_speed - 3.5*error;
		if(basic_speed<30) basic_speed = 30;
	}
	else 
	{
		kp=0;
		kd=0;
		beep_briefly_on();
		basic_speed = 0;
		if_car_move = 2;//停车
	}
	*/
	//6.17
	
	
	/*if(error<10)
	{
		beep_OFF();
		basic_speed = average_speed;
		kp=5;
		kd=15;
	}
	else if(error<20)	//误差值在100内
	{
		beep_OFF();
		basic_speed = average_speed;
		kp=5;
		kd=15;
	}
	else if(error<35)
	{
		beep_ON();
		basic_speed = average_speed-60;
		kp=5;
		kd=25;
	}
	else if(error<50)
	{
		beep_ON();
		basic_speed = average_speed-100;
		kp=5;
		kd=25;
	}
	else if(error<65)
	{
		beep_ON();
		basic_speed = average_speed-150;
		kp=5;
		kd=30;
	}
	else 
	{
		beep_briefly_on();
		basic_speed = 0;
		kp=0;
		kd=0;
	}
	*/
}







