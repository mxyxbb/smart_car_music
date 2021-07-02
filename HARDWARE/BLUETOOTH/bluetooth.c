#include "include.h"
#include "shell_port.h"
#include "NIMING/data_transfer.h"

void UART1_IRQHandler(void)
{
  //�����ж�
  if(UART_GetFlagStatus(UART1, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
    /* �û����� */
		recv_buf=UART_GetChar(UART1);
//		shellHandler(&shell, recv_buf);
		ANO_DT_Data_Receive_Prepare(recv_buf);

  }
}
//˫��ͨ�ŵĽ��տ�

/*************************************************************************
*  �������ƣ�void UART_RX_IRQ_Config(UART_NAME_t UART_x,u8 status)
*  ����˵�����������ڽ����ж�
*  ����˵����
* @param    UART_x       ����ͨ��
* @param    status      ʹ�ܻ��ֹ
*  �������أ��ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_RX_IRQ_Config(UART_1, ENABLE);  //��������1�����ж�
*************************************************************************/
void UART_RX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
{
  UART_ITConfig(UARTx, UART_IT_RXIEN, ENABLE);
  if(UART1 == UARTx) nvic_init(UART1_IRQn, 0, 0, status);
  else if(UART2 == UARTx) nvic_init(UART2_IRQn, 0, 0, status); 
}

/*************************************************************************
*  �������ƣ�void UART_RX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
*  ����˵�����������ڽ����ж�
*  ����˵����
* @param    UART_x       ����ͨ��
* @param    status      ʹ�ܻ��ֹ
*  �������أ��ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_RX_IRQ_Config(UART_1, ENABLE);  //��������1�����ж�
*************************************************************************/
void UART_TX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
{
  UART_ITConfig(UARTx, UART_IT_TXIEN, ENABLE);
  if(UART1 == UARTx) nvic_init(UART1_IRQn, 0, 0, status);
  else if(UART2 == UARTx) nvic_init(UART2_IRQn, 0, 0, status);  
}
/*************************************************************************
*  �������ƣ�void UART_PutChar(UART_t  uratn, char ch)
*  ����˵����UART�����ֽں���,ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ ch    �� Ҫ��ӡ���ַ�
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutChar(UART0, 'a');  //��ӡ�ַ�a
*************************************************************************/
void UART_PutChar(UART_TypeDef* UARTx,char ch)
{
  UART_SendData(UARTx, ch);
  while (UART_GetFlagStatus(UARTx, UART_FLAG_TXEPT) == RESET);
}

/*************************************************************************
*  �������ƣ�void UART_PutStr(UART_t  uratn, char *str)
*  ����˵����UART�����ַ�������(�� NULL ֹͣ����),ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ str   �� Ҫ��ӡ���ַ�����ַ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutStr(UART3, "123456789"); //����9���ֽ�
*************************************************************************/
void UART_PutStr(UART_TypeDef* UARTx,char *str)
{
  while(*str)
  {
    UART_PutChar(UARTx,*str++);
  }
}
/*************************************************************************
*  �������ƣ�char UART_GetChar(UART_t  uratn)
*  ����˵����UART��ȡ�ֽ� ʹ��ǰ���ȳ�ʼ����Ӧ���� ����ǰ��ȷ���н��յ����� �����ȴ����ݵ���
*  ����˵����uratn �� UART0 - UART3
*  �������أ���ȡ�ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_GetChar(UART3); //����һ���ַ�
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
	
  /*---------------------------- �������� -----------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//UART1 TX PA9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //���ù���1
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//UART1 RX PA10
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//���ù���1
	
	/*---------------------------- �������� -----------------------*/
	UART_InitStructure.UART_BaudRate = 115200;//������
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;//1ֹͣλ
  UART_InitStructure.UART_Parity = UART_Parity_No;//����żУ��
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_InitStructure.UART_Mode = UART_Mode_Tx | UART_Mode_Rx;
  UART_Init(UART1, &UART_InitStructure);
  
	//ʹ�ܴ���
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
		sprintf(str,"%.2f",float_num_a);     //������ת��Ϊ�ַ�������
  	UART_PutStr(UART1,str);
		UART_PutChar(UART1,' '); 
		
		sprintf(str,"%.2f",float_num_b);
		UART_PutStr(UART1,str);
    UART_PutStr(UART1,"\r\n");      //�����ַ�������λ��
		
    delayms(200);              //��ʱ�ȴ�
  }
}

void send_speed_and_pwm()
{
	char str[20];
	sprintf(str,"%.2f",now_speed_left);     //������ת��Ϊ�ַ�������
  UART_PutStr(UART1,str);
	UART_PutChar(UART1,' '); 
	sprintf(str,"%.2f",now_speed_right);     //������ת��Ϊ�ַ�������
  UART_PutStr(UART1,str);
	UART_PutChar(UART1,' ');
	sprintf(str,"%d",left_pwm);     //����ת��Ϊ�ַ�������
  UART_PutStr(UART1,str);
	UART_PutChar(UART1,' '); 
	sprintf(str,"%d",right_pwm);     //����ת��Ϊ�ַ�������
  UART_PutStr(UART1,str);
	UART_PutStr(UART1,"\r\n");
}
void send_error()
{
	
}


