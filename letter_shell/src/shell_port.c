/**
 * @brief shell��ֲ��STM32F407ʱ�Ľӿ�ʵ��
 * @author mculover666
 * @date 2020/03/27 
*/

#include "shell.h"

//#include "usart.h"
#include "include.h"
#include "shell_port.h"
#include <stdio.h>

/* 1. ����shell���󣬿���shell������ */
Shell shell;
char shell_buffer[512];
uint8_t recv_buf = 0;

/* 2. �Լ�ʵ��shellд���� */

//shellд����ԭ�ͣ�typedef void (*shellWrite)(const char);
void User_Shell_Write(const char ch)
{
	#ifdef SHELL_USE_HAL
		//����STM32 HAL�� API ʹ�ò�ѯ��ʽ����
		HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xFFFF);
	#endif
	#ifdef SHELL_USE_STD
		UART_SendData(UART1, ch);
		while (UART_GetFlagStatus(UART1, UART_FLAG_TXEPT) == RESET);
	#endif
}

/* 3. ��д��ʼ������ */
void User_Shell_Init(void)
{
 //ע���Լ�ʵ�ֵ�д����
    shell.write = User_Shell_Write;
 
 //����shell��ʼ������
    shellInit(&shell, shell_buffer, 512);
}
