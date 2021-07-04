#include "multi_button/multi_button.h"
#include "include.h"
#include "buzzerDriver.h"
#include "mymath.h"
#include "GPIO.h"

uint8_t read_button1_GPIO();
uint8_t read_button2_GPIO();


void BTN1_SINGLE_Click_Handler(void* btn);
void BTN2_SINGLE_Click_Handler(void* btn);
void BTN1_LONG_PRESS_START_Handler(void* btn);
void BTN2_LONG_PRESS_START_Handler(void* btn);
void BTN1_DOUBLE_Click_Handler(void* btn);
void BTN2_DOUBLE_Click_Handler(void* btn);
void BTN1_LONG_PRESS_HOLD_Handler(void* btn);
void BTN2_LONG_PRESS_HOLD_Handler(void* btn);	

struct Button button1;
struct Button button2;

void my_button_init()
{
	button_init(&button1, read_button1_GPIO, 0);//multibtn初始化
	button_init(&button2, read_button2_GPIO, 0);//multibtn初始化
	
//	button_attach(&button1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);
//	button_attach(&btn1, PRESS_UP,         BTN1_PRESS_UP_Handler);
//	button_attach(&btn1, PRESS_REPEAT,     BTN1_PRESS_REPEAT_Handler);
	button_attach(&button1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);
	button_attach(&button1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);
//	button_attach(&button1, LONG_PRESS_START, BTN1_LONG_PRESS_START_Handler);
	button_attach(&button1, LONG_PRESS_HOLD,  BTN1_LONG_PRESS_HOLD_Handler);

//	button_attach(&button2, PRESS_DOWN,       BTN2_PRESS_DOWN_Handler);
//	button_attach(&btn2, PRESS_UP,         BTN2_PRESS_UP_Handler);
//	button_attach(&btn2, PRESS_REPEAT,     BTN2_PRESS_REPEAT_Handler);
	button_attach(&button2, SINGLE_CLICK,     BTN2_SINGLE_Click_Handler);
	button_attach(&button2, DOUBLE_CLICK,     BTN2_DOUBLE_Click_Handler);
//	button_attach(&button2, LONG_PRESS_START, BTN2_LONG_PRESS_START_Handler);
	button_attach(&button2, LONG_PRESS_HOLD,  BTN2_LONG_PRESS_HOLD_Handler);
	button_start(&button1);
	button_start(&button2);
}

void BTN1_SINGLE_Click_Handler(void* btn)
{
	buzzer_flag=1;
	music5_play_flag=1;
	if_key=1;
	
}

void BTN2_SINGLE_Click_Handler(void* btn)
{
	buzzer_flag=1;
	if_key=2;
}

void BTN1_DOUBLE_Click_Handler(void* btn)
{
	
	if_music1=!if_music1;
	if_music2=0;
}
void BTN2_DOUBLE_Click_Handler(void* btn)
{
	if_music2=!if_music2;
	if_music1=0;
	
}


//void BTN1_LONG_PRESS_START_Handler(void* btn)
//{
//	buzzer_flag=1;

//}

//void BTN2_LONG_PRESS_START_Handler(void* btn)
//{
//	buzzer_flag=1;

//}

u8 slow_factor;

void BTN1_LONG_PRESS_HOLD_Handler(void* btn)
{
//	buzzer_flag=1;
	slow_factor++;
	if(slow_factor%5==0)
	{
//		buzzer_flag=1;
		music_volume--;
		slow_factor=0;
	}
	if(music_volume<3) 
	{
		buzzer_flag=1;
		music_volume=2;
	}
}

void BTN2_LONG_PRESS_HOLD_Handler(void* btn)
{
//	buzzer_flag=1;
	slow_factor++;
	if(slow_factor%5==0)
	{
//		buzzer_flag=1;
		music_volume++;
		slow_factor=0;
	}
	if(music_volume>60)
	{
		buzzer_flag=1;
		music_volume=60;
	}
}





uint8_t read_button1_GPIO()
{
	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3);
}
uint8_t read_button2_GPIO()
{
	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
}