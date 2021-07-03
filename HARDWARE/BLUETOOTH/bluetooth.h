#ifndef __BT_H_
#define __BT_H_

#include "include.h"

void UART_RX_IRQ_Config(UART_TypeDef* UARTx,u8 status);
void UART_TX_IRQ_Config(UART_TypeDef* UARTx,u8 status);
void UART_PutChar(UART_TypeDef* UARTx,char ch);
void UART_PutStr(UART_TypeDef* UARTx,char *str);
char UART_GetChar(UART_TypeDef* UARTx);
void Test_Bluetooth(void);
void Bluetooth_Init(void);
void send_speed_and_pwm(void);
void send_error(void);



extern int left_pwm ;
extern int right_pwm ;
extern float now_speed_left;
extern float now_speed_right;
extern int left_error;


#endif
