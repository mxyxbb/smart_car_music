#include "include.h"
#include "shell_port.h"
#include "buzzerDriver.h"
#include <I2C_MPU9250.h>
#include "multi_button/multi_button.h"
#include "angle.h"
#include "NIMING/data_transfer.h"
#include "BALANCE/balance.h"

char str_main[255];
u32 count_main;
u8 count_mode_main;

int main(void)
{
	GPIO_LED_Init();
	GPIO_KEY_Init();
	GPIO_beep_init();//初始化LED、蜂鸣器、按键
	PWM_Init();
	mysys_init();
	adc_init();
	TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
	encoder_init();
	Bluetooth_Init();  //初始化蓝牙
	MPU9250_Set_LPF(10);
//	User_Shell_Init();
	MPU9250_Init();
	my_button_init();
	beep_briefly_on();
	TIMER_InitConfig(TIMER_16, 10);//定时器16每隔10ms进入一次中断
	
	while (1)
  {
		ANO_DT_Data_Exchange();
		delayms(1);
//		sprintf(str_main, "C1:%5f", Encoder_Integral); 
//    TFTSPI_P8X16Str(1, 1, str_main, u16PURPLE, u16BLACK);   // 字符串显示
//		start_or_stop();
  }
	
}


