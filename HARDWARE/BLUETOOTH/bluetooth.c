#include "include.h"
#include "shell_port.h"
#include "NIMING/data_transfer.h"

void UART1_IRQHandler(void)
{
  //接收中断
  if(UART_GetFlagStatus(UART1, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
    /* 用户代码 */
		recv_buf=UART_GetChar(UART1);
//		shellHandler(&shell, recv_buf);
		ANO_DT_Data_Receive_Prepare(recv_buf);

  }
}
//双机通信的接收口

/*************************************************************************
*  函数名称：void UART_RX_IRQ_Config(UART_NAME_t UART_x,u8 status)
*  功能说明：开启串口接收中断
*  参数说明：
* @param    UART_x       串口通道
* @param    status      使能或禁止
*  函数返回：字节
*  修改时间：2020年3月10日
*  备    注：UART_RX_IRQ_Config(UART_1, ENABLE);  //开启串口1接收中断
*************************************************************************/
void UART_RX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
{
  UART_ITConfig(UARTx, UART_IT_RXIEN, ENABLE);
  if(UART1 == UARTx) nvic_init(UART1_IRQn, 0, 0, status);
  else if(UART2 == UARTx) nvic_init(UART2_IRQn, 0, 0, status); 
}

/*************************************************************************
*  函数名称：void UART_RX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
*  功能说明：开启串口接收中断
*  参数说明：
* @param    UART_x       串口通道
* @param    status      使能或禁止
*  函数返回：字节
*  修改时间：2020年3月10日
*  备    注：UART_RX_IRQ_Config(UART_1, ENABLE);  //开启串口1接收中断
*************************************************************************/
void UART_TX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
{
  UART_ITConfig(UARTx, UART_IT_TXIEN, ENABLE);
  if(UART1 == UARTx) nvic_init(UART1_IRQn, 0, 0, status);
  else if(UART2 == UARTx) nvic_init(UART2_IRQn, 0, 0, status);  
}
/*************************************************************************
*  函数名称：void UART_PutChar(UART_t  uratn, char ch)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ ch    ： 要打印的字符
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutChar(UART0, 'a');  //打印字符a
*************************************************************************/
void UART_PutChar(UART_TypeDef* UARTx,char ch)
{
  UART_SendData(UARTx, ch);
  while (UART_GetFlagStatus(UARTx, UART_FLAG_TXEPT) == RESET);
}

/*************************************************************************
*  函数名称：void UART_PutStr(UART_t  uratn, char *str)
*  功能说明：UART发送字符串函数(遇 NULL 停止发送),使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ str   ： 要打印的字符串地址
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutStr(UART3, "123456789"); //发送9个字节
*************************************************************************/
void UART_PutStr(UART_TypeDef* UARTx,char *str)
{
  while(*str)
  {
    UART_PutChar(UARTx,*str++);
  }
}
/*************************************************************************
*  函数名称：char UART_GetChar(UART_t  uratn)
*  功能说明：UART读取字节 使用前请先初始化对应串口 调用前请确认有接收到数据 否则会等待数据到来
*  参数说明：uratn ： UART0 - UART3
*  函数返回：读取字节
*  修改时间：2020年3月10日
*  备    注：UART_GetChar(UART3); //接收一个字符
*************************************************************************/
char UART_GetChar(UART_TypeDef* UARTx)
{  
  return (UART_ReceiveData(UARTx));
}

/*******************************************************************************
* Function Name  : _write
* Description    : Support Printf Function
* Input          : *buf: UART send Data.
*                  size: Data length
* Return         : size: Data length
*******************************************************************************/
int _write(int fd, char *buf, int size)
{
  int i;
  
  for(i=0; i<size; i++)
  {
#if (DEBUG == DEBUG_UART1)
    while (UART_GetFlagStatus(UART1, UART_FLAG_TXEPT) == RESET);
    UART_PutChar(UART1, *buf++);
#elif (DEBUG == DEBUG_UART2)
    while (UART_GetFlagStatus(UART2, UART_FLAG_TXEPT) == RESET);
    UART_PutChar(UART2, *buf++);
#endif
  }
  return size;
}

void Bluetooth_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	UART_InitTypeDef UART_InitStructure;
	
  /*---------------------------- 引脚配置 -----------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//UART1 TX PA9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //复用功能1
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//UART1 RX PA10
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//复用功能1
	
	/*---------------------------- 串口配置 -----------------------*/
	UART_InitStructure.UART_BaudRate = 115200;//波特率
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;//1停止位
  UART_InitStructure.UART_Parity = UART_Parity_No;//无奇偶校验
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_InitStructure.UART_Mode = UART_Mode_Tx | UART_Mode_Rx;
  UART_Init(UART1, &UART_InitStructure);
  
	//使能串口
  UART_RX_IRQ_Config(UART1,ENABLE);
  UART_Cmd(UART1, ENABLE);
}
	
void Test_Bluetooth(void)
{
	char str[20];
	float float_num_a = 0;
	float float_num_b = 0;
	Bluetooth_Init();
  while(1)
  {
		float_num_a += 1.7;
		float_num_b += 2.2;
		sprintf(str,"%.2f",float_num_a);     //浮点数转换为字符串函数
  	UART_PutStr(UART1,str);
		UART_PutChar(UART1,' '); 
		
		sprintf(str,"%.2f",float_num_b);
		UART_PutStr(UART1,str);
    UART_PutStr(UART1,"\r\n");      //发送字符串到上位机
		
    delayms(200);              //延时等待
  }
}

void send_speed_and_pwm()
{
	char str[20];
	sprintf(str,"%.2f",now_speed_left);     //浮点数转换为字符串函数
  UART_PutStr(UART1,str);
	UART_PutChar(UART1,' '); 
	sprintf(str,"%.2f",now_speed_right);     //浮点数转换为字符串函数
  UART_PutStr(UART1,str);
	UART_PutChar(UART1,' ');
	sprintf(str,"%d",left_pwm);     //整形转换为字符串函数
  UART_PutStr(UART1,str);
	UART_PutChar(UART1,' '); 
	sprintf(str,"%d",right_pwm);     //整形转换为字符串函数
  UART_PutStr(UART1,str);
	UART_PutStr(UART1,"\r\n");
}
void send_error()
{
	
}


