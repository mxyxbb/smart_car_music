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
	GPIO_beep_init();//��ʼ��LED��������������
	PWM_Init();
	mysys_init();
	adc_init();
	TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ
	encoder_init();
	Bluetooth_Init();  //��ʼ������
	MPU9250_Set_LPF(10);
//	User_Shell_Init();
	MPU9250_Init();
	my_button_init();
	beep_briefly_on();
	TIMER_InitConfig(TIMER_16, 10);//��ʱ��16ÿ��10ms����һ���ж�
	
	while (1)
  {
		ANO_DT_Data_Exchange();
		delayms(1);
//		sprintf(str_main, "C1:%5f", Encoder_Integral); 
//    TFTSPI_P8X16Str(1, 1, str_main, u16PURPLE, u16BLACK);   // �ַ�����ʾ
//		start_or_stop();
  }
	
}


