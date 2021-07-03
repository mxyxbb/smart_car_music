#include "hellodata/hellodata.h"
#include "bluetooth.h"


void HelloData_Send_Data(uint8_t *dataToSend , uint8_t length)//可修改到其他串口
{
	
	for(u8 i=0;i<length;i++)
	{
		UART_SendData(UART1, dataToSend[i]);
		while (UART_GetFlagStatus(UART1, UART_FLAG_TXEPT) == RESET);
	}
}

uint8_t  USART_TX_BUF[200]; //发送缓冲,最大200字节，不能太小，如果你的内容太长会访问非法内存
#define _DEBUG_  0 	//串口打印宏函数开关，1是开，0是关，调试的时候开，调式完毕就可以一键关闭
/*------------------DEBUG DEFINE BEGIN------------------------*/
#if _DEBUG_
#define ps(...)  HelloData_Send_Data(USART_TX_BUF,sprintf((char *)USART_TX_BUF,__VA_ARGS__))
#else
#define ps(...)
#endif
/*------------------DEBUG DEFINE END------------------------*/

#define NORMAL 0
#define BIG 1
#define SMALL 2

uint8_t count_type=0;//旧类型，0--正常，1--异常大，2--异常小
uint8_t count_type2=0;//新类型0--正常，1--异常大，2--异常小
uint16_t a[12]={0};//正常数组
uint16_t ab[12]={0};//不正常数组
uint8_t a_index=0;//正常数组的索引
uint8_t ab_index=0;//不正常数组的索引
float count=0;
uint8_t ab_count=0;
uint8_t full=0;
float hello_aver=0;
uint32_t timehere=0;

uint8_t judgeNormal()
{
  uint8_t type=0;
  ps("aver/5=");
  ps("%f",hello_aver/5); 
  ps(",aver*5=");
  ps("%f",hello_aver*5); 
  ps(",count=");
  ps("%f",count); 
 
  if((count >= hello_aver/5) && (count <= 5*hello_aver))//正常
    type=0;
  else if(count > 5*hello_aver)//异常大
    type=1;
  else if(count < hello_aver/5)//异常小
    type=2;
  ps(",type=");
  ps("%d\r\n",type);
  
  return type;
}

void filla()
{
  a[a_index++]=count;  
  if(a_index==12){
      a_index=0;
      full=1;
      ps("normal group is full\n\r");
  }
  ps("fill in normal group\n\r");
}
void fillab()
{
  ab[ab_index++]=count;  
  if(ab_index==12)
      ab_index=0;
  ps("fill in abnormal group\n\r");
}
void cleara()
{
  for(uint8_t k=0;k<12;k++)
    a[k]=0;
  a_index=0;
  ps("clear normal group\n\r");
}
void clearab()
{
  for(uint8_t k=0;k<12;k++)
    ab[k]=0;
  ab_count=0;
  ab_index=0;
  ps("clear abnormal group\n\r");
}
void aba()//用异常替换正常
{
  for(uint8_t k=0;k<12;k++)
    a[k]=ab[k];
  a_index=ab_index;
  full=0;
  count_type=0;
  count_type2=0;
}
void printa()
{
  ps("normal group: ");
  for(uint8_t k=0;k<12;k++)
  {
    ps("%d",a[k]);
    ps(",");
  }
  ps("\n\r");
}
void printab()
{
  ps("abnormal group: ");
  for(uint8_t k=0;k<12;k++)
  {
    ps("%d",ab[k]);
    ps(",");
  }
  ps("\n\r");
}

void hello(uint32_t count)
{

  static uint8_t first=0;
  uint32_t sum=0;


  if(first==0)//第一次
  { 
    ps("first calculate, count=");
    ps("%d\n\r",count);
    first=1;
    a[a_index++]=count;
  }
  else//第二次及以后
  {
    for(uint8_t k=0;k<12;k++)//对数组求和
      sum+=a[k];
    if(full)
      hello_aver=(float)((sum)/12);
     else
      hello_aver=(float)((sum)/a_index);
//    ps("加入新数据前的数组平均数为");
    ps("normal group average value before the new count= ");
    ps("%f\n\r",hello_aver);

    //判断计数是否异常
    count_type2=judgeNormal();
    if (count_type2==NORMAL)
    {
      ps("count is NORMAL!\n\r");
      if (count_type!=NORMAL)
      { 
        ps("last count is abnormal\n\r");
        clearab();
        
      }
      else
      {
        ps("last count is normal\n\r");
        filla();
        
      }
    }
    else
    {
      ab_count++;
      ps("this count is abnormal\n\r");
        if (count_type!=NORMAL)
        {
          ps("last count is abnormal\n\r");
          if (count_type!=count_type2)
            clearab();
          else
            fillab();
        }
        else
        {
          ps("last count is normal\n\r");
          fillab();
        }
      if (ab_count==3)
      {
//        Serial.println("3次同类型异常，要替换正常组。");
        ps("3 same error become normal\n\r");
        ps("abnormal group: ");
        for(uint8_t k=0;k<12;k++)
        {
          ps("%d",ab[k]);
          ps(",");
        }
        ps("\n\r");
//        ps("替换前的正常组：");
        ps("normal group before change:");
        for(uint8_t k=0;k<12;k++)
        {
          ps("%d",a[k]);
          ps(",");
        }
        ps("\n\r");
        
        cleara();
        aba();
//        Serial.print("替换后的正常组：");
				ps("normal group after change:");
        for(uint8_t k=0;k<12;k++)
        {
          ps("%d",a[k]);
          ps(",");
        }
        ps("\n\r");
        clearab();
      }
    }
  }
  count_type=count_type2;
}

void getcount()
{
  count++;
}

/*
void setup()   
{  
  pinMode(3,INPUT);
  Serial.begin(9600);
  attachInterrupt(3,getcount,FALLING); 
  noInterrupts();
}

void loop()
{
  count=0;
  timehere = millis();
  interrupts();
  while(millis() - timehere<5000)
  {delay(1);}
  Serial.println("--------新的5s--------------");
  Serial.print("最近5s脉冲数：");
  Serial.println(count);
  noInterrupts();
  Serial.println("hellocount");
  hello(count);
  printa();
  printab();
}
*/
