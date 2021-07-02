/*
ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
*/
#ifndef __LQ_SOFTIIC_H
#define __LQ_SOFTIIC_H

#include "stdint.h"
#include "include.h"


#define MPU9250_SCL_PIN   GPIOB,GPIO_Pin_8   /*!< SCCB SCL �ܽ� */
#define MPU9250_SDA_PIN   GPIOB,GPIO_Pin_9   /*!< SCCB SDA �ܽ� */

#define SDA_OUT        MPU9250_SDA_Out();
#define SDA_IN         MPU9250_SDA_In();

#define IIC_SCL_H  GPIO_SetBits(MPU9250_SCL_PIN);        //��������ߵ�ƽ
#define IIC_SCL_L  GPIO_ResetBits(MPU9250_SCL_PIN);      //��������͵�ƽ

#define IIC_SDA_H  GPIO_SetBits(MPU9250_SDA_PIN);        //��������ߵ�ƽ
#define IIC_SDA_L  GPIO_ResetBits(MPU9250_SDA_PIN);      //��������͵�ƽ

#define IIC_SDA_READ  GPIO_ReadInputDataBit(MPU9250_SDA_PIN)//��ȡ�����ϵ�����״̬


void IIC_Start(void);			    //����IIC��ʼ�ź�
void IIC_Stop(void);	  	        //����IICֹͣ�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
unsigned char IIC_WaitAck(void); 		    //IIC�ȴ�ACK�ź�
void IIC_SendByte(unsigned char data_t);    //IIC����һ���ֽ�
unsigned char IIC_ReadByte(unsigned char ack);      //IIC��ȡһ���ֽ�


void MPU9250_SCL_Out(void);
void MPU9250_SDA_Out(void);
void MPU9250_SDA_In(void);

void IIC_Init(void);
unsigned char IIC_ReadByteFromSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char *buf);
unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t);
unsigned char IIC_WriteByteToSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char buf);
unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data_t);
#endif
