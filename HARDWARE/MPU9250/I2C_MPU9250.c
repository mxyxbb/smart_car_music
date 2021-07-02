#include <I2C_MPU9250.h>
#include <include.h>
#include <SOFTI2C.h>
#include <stdio.h>
#include <math.h>
/**
  * @brief    ����ȷ��ʱ
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      delayms_mpu9250(100);
  *
  * @date     2020/12/10 ������
*/
void delayms_mpu9250(uint16_t ms)
{
    int i;
  for (i = 0; i < 300; ++i)
  {
    __asm("NOP"); /* delay */
  }
//	while(ms--)
//	{
//		uint16_t  i = 300;
//		while(i--)
//		{
//			NOP(50);
//		}
//	}
}


/**
  * @brief    ��ʼ��MPU9250
  *
  * @param    ��
  *
  * @return   0����ʼ���ɹ�   1��ʧ��
  *
  * @note     ʹ��ǰ�ȳ�ʼ��IIC�ӿ�
  *
  * @see      MPU9250_Init();
  *
  * @date     2020/12/15 ���ڶ�
  */
unsigned char MPU9250_Init(void)
{
    int  res;
    IIC_Init();                                           //MPU9250 ֧��400K IIC

    res = 0;
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X80);//��λMPU9250
    delayms_mpu9250(100);  //��ʱ100ms
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X00);//����MPU9250
    res += MPU9250_Set_Gyro_Fsr(3);                                //�����Ǵ�����,��2000dps
    res += MPU9250_Set_Accel_Fsr(0);                               //���ٶȴ�����,��2g
    res += MPU9250_Set_Rate(1000);                                 //���ò�����1000Hz
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_INT_EN_REG,0X00);   //�ر������ж�
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_CFG_REG,0x02);      //�������ֵ�ͨ�˲���   98hz
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_USER_CTRL_REG,0X00);//I2C��ģʽ�ر�
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_FIFO_EN_REG,0X00);  //�ر�FIFO
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_INTBP_CFG_REG,0X82);//INT���ŵ͵�ƽ��Ч������bypassģʽ
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X01);//����CLKSEL,PLL X��Ϊ�ο�
    res += MPU9250_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT2_REG,0X00);//���ٶ��������Ƕ�����

		
/*
    if(res == 0)  //����Ĵ�����д��ɹ�
    {
        //UART_PutStr(UART1, "MPU9250 set is OK!\r\n");
    }
    else{
        //UART_PutStr(UART1, "MPU9250 set is error!\r\n");
        return 1;
    }

    res = MPU9250_Read_Byte(MPU9250_ADDR,WHO_AM_I);     //��ȡMPU6500��ID
    if( (res == MPU6500_ID1) || (res == MPU6500_ID2) )  //����ID��ȷ
        UART_PutStr(UART1, "MPU6500_ID1 is OK!\r\n");
		
    else
    {
        //printf("ID=%#X\r\n",res);
        //printf("MPU9250 is fail!\n");
//        UART_PutStr(UART1,"MPU6500_ID1 is error!\r\n");
//        UART_PutChar(UART1, res);
//        UART_PutStr(UART1,"\r\n");
//        UART_PutChar(UART1, MPU6500_ID1);
//        UART_PutStr(UART1,"\r\n");
        while(1){};
    }

    res = MPU9250_Read_Byte(AK8963_ADDR,MPU_MAG_WIA);   //��ȡMPU9250��ID
    if(res == AK8963_ID)                                //����ID��ȷ
    {
        res += MPU9250_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL2_REG,0X01);//�������
        delayms_mpu9250(20);
        res += MPU9250_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL1_REG,0X16);//����Ϊ����ģʽ�����16bit
        //UART_PutStr(UART1, "AK8963_ID is OK!\r\n");
    }
    else
    {
        //printf("ID=%#X\r\n",res);
        //printf("MPU9250 is fail!\n");
//        UART_PutStr(UART1,"AK8963_ID is error!\r\n");
//        UART_PutChar(UART1, res);
//        UART_PutStr(UART1,"\r\n");
//        UART_PutChar(UART1, AK8963_ID);
//        UART_PutStr(UART1,"\r\n");
        while(1);
    }
*/
    return 0;
}




/**
  * @brief    ���������ǲ�����Χ
  *
  * @param    fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
  *
  * @return   0 �����óɹ�
  *
  * @note     ��
  *
  * @see      MPU9250_Set_Gyro_Fsr(3);		  //�����Ǵ�����,��2000dps
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU9250_Write_Byte(MPU9250_ADDR,MPU_GYRO_CFG_REG,fsr<<3);
}



/**
  * @brief    ���ü��ٶȼƲ�����Χ
  *
  * @param    fsr:0,��2g;1,��4g;2,��8g;3,��16g
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU9250_Set_Accel_Fsr(1);		  //���ٶȴ�����,��4g
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU9250_Write_Byte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);
}



/**
  * @brief    �������ֵ�ͨ�˲�
  *
  * @param    lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU9250_Set_LPF(100);
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Set_LPF(uint16_t lpf)
{
	unsigned char  data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6;
	return MPU9250_Write_Byte(MPU9250_ADDR,MPU_CFG_REG,data);//�������ֵ�ͨ�˲���
}




/**
  * @brief    ���ò�����
  *
  * @param    rate:4~1000(Hz)
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU9250_Set_Rate(1000);       	   //���ò�����1000Hz
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Set_Rate(uint16_t rate)
{
	unsigned char  data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data = 1000/rate-1;
	MPU9250_Write_Byte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,data);	    //�������ֵ�ͨ�˲���
 	return MPU9250_Set_LPF(rate/2);	                                //�Զ�����LPFΪ�����ʵ�һ��
}



/**
  * @brief    ��ȡ�¶�ֵ
  *
  * @param    ��
  *
  * @return   �¶�ֵ(������100��)
  *
  * @note     ��
  *
  * @see      signed short temp = MPU9250_Get_Temperature();
  *
  * @date     2020/12/10 ������
  */
short MPU9250_Get_Temperature(void)
{
    unsigned char  buf[2];
    short raw;
	float temp;
	MPU9250_Read_Len(MPU9250_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((uint16_t)buf[0]<<8)|buf[1];
    temp=21+((double)raw)/333.87;
    return (short)temp*100;
}





/**
  * @brief    ��ȡ������ֵ
  *
  * @param    gx,gy,gz:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[3];
  * @see      MPU9250_Get_Gyroscope(&data[0], &data[1], &data[2]);
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Get_Gyroscope(signed short *gx,signed short *gy,signed short *gz)
{
    unsigned char  buf[6],res;
	res=MPU9250_Read_Len(MPU9250_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];
		*gy=((uint16_t)buf[2]<<8)|buf[3];
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	}
    return res;
}


/**
  * @brief    ��ȡ���ٶ�ֵ
  *
  * @param    ax,ay,az:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[3];
  * @see      MPU9250_Get_Accelerometer(&data[0], &data[1], &data[2]);
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Get_Accelerometer(signed short *ax,signed short *ay,signed short *az)
{
    unsigned char  buf[6],res;
	res=MPU9250_Read_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];
		*ay=((uint16_t)buf[2]<<8)|buf[3];
		*az=((uint16_t)buf[4]<<8)|buf[5];
	}
    return res;
}


/**
  * @brief    ��ȡ������ֵ
  *
  * @param    mx,my,mz:��������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[3];
  * @see      MPU9250_Get_Magnetometer(&data[0], &data[1], &data[2]);
  *
  * @date     2020/12/15 ���ڶ�
  */
unsigned char MPU9250_Get_Magnetometer(short *mx,short *my,short *mz)
{

    unsigned char  buf[6],res;
    res=MPU9250_Read_Len(AK8963_ADDR,MPU_MAG_XOUTL_REG,6,buf);
    if(res==0)
    {
        *mx=((uint16_t)buf[1]<<8)|buf[0];
        *my=((uint16_t)buf[3]<<8)|buf[2];
        *mz=((uint16_t)buf[5]<<8)|buf[4];
    }
    //MPU9250_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL1_REG,0X11);
    MPU9250_Read_Byte(AK8963_ADDR,MPU_MAG_ST2_REG);
    return res;
}

/**
  * @brief    ��ȡ ���ٶ�ֵ ���ٶ�ֵ
  *
  * @param    ax,ay,az:������x,y,z���ԭʼ����(������)
  * @param    gx,gy,gz:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[6];
  * @see      MPU9250_Get_Raw_data(&data[0], &data[1], &data[2],&data[3], &data[4], &data[5]);
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Get_Raw_data(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
{
    unsigned char  buf[14],res;
	res=MPU9250_Read_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];
		*ay=((uint16_t)buf[2]<<8)|buf[3];
		*az=((uint16_t)buf[4]<<8)|buf[5];
        *gx=((uint16_t)buf[8]<<8)|buf[9];
		*gy=((uint16_t)buf[10]<<8)|buf[11];
		*gz=((uint16_t)buf[12]<<8)|buf[13];
	}
    return res;
}

/**
  * @brief    IIC ����д
  *
  * @param    addr:������ַ
  * @param    reg :Ҫд��ļĴ�����ַ
  * @param    len :Ҫд��ĳ���
  * @param    buf :д�뵽�����ݴ洢��
  *
  * @return   0 ��д��ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      unsigned char buf[14];
  * @see      MPU9250_Write_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_WriteMultByteToSlave(addr<<1, reg, len, buf);
}


/**
  * @brief    IIC ������
  *
  * @param    addr:������ַ
  * @param    reg :Ҫ��ȡ�ļĴ�����ַ
  * @param    len :Ҫ��ȡ�ĳ���
  * @param    buf :��ȡ�������ݴ洢��
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      unsigned char buf[14];
  * @see      MPU9250_Read_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);
}



/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    addr  :������ַ
  * @param    reg   :�Ĵ�����ַ
  * @param    value :Ҫд���ֵ
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      MPU9250_Write_Byte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,1);
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Write_Byte(unsigned char addr,unsigned char reg,unsigned char value)
{
    return IIC_WriteByteToSlave(addr<<1, reg, value);
}


/**
  * @brief    IIC ��һ���Ĵ���
  *
  * @param    addr  :������ַ
  * @param    reg   :�Ĵ�����ַ
  *
  * @return   ��ȡ��ֵ
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      MPU9250_Read_Byte(MPU9250_ADDR,WHO_AM_I);
  *
  * @date     2020/12/10 ������
  */
unsigned char MPU9250_Read_Byte(unsigned char addr,unsigned char reg)
{
    unsigned char value[1];
    IIC_ReadByteFromSlave(addr<<1, reg, &value[0]);
    return value[0];
}

// ͨ�����ٶȼƻ�ȡ��������
float MPU9250_Get_Angle(short accx, short accy, short accz, char choose)
{
    double temp;
    float res = 0;
    switch(choose)
    {
        case 0 ://����Ȼx��ĽǶ�
            temp = (float)accx / sqrt( (accy * accy + accz * accz) );
            res = atan(temp);
            break;
        case 1 ://����Ȼy��ĽǶ�
            temp = (float)accy / sqrt((accx * accx + accz * accz));
            res = atan(temp);
            break;
        case 2 ://����Ȼz��ĽǶ�
            temp = (float)accz / sqrt(accx * accx + accy * accy);
            res = atan(temp);
            break;
    }
    return res * 1800 / 3.1415;
}

// inv_mpu.c��Ҫ
void mget_ms(unsigned long *count)
{

}

//���Գ���
//��λΪ0.1����ÿ��

void Test_MPU9250(void)
{

 
}
