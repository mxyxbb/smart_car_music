#include "angle.h"
#include <math.h>
#include <I2C_MPU9250.h>
#include "shell_port.h"

float Angle_Balance,Gyro_Balance,Gyro_Turn; //ƽ����� ƽ�������� ת��������
float Acceleration_Z;                       //Z����ٶȼ�  

float K1 =0.02; 
float angle;

mpu mpu6050;


int debug_angle;
SHELL_EXPORT_VAR(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_VAR_INT), a, &debug_angle, debug_angle);

void angle_hanlder (void)
{
	MPU9250_Get_Raw_data(&(mpu6050.Acc.x),&(mpu6050.Acc.y),&(mpu6050.Acc.z),&(mpu6050.Gyro.x),&(mpu6050.Gyro.y),&(mpu6050.Gyro.z));   //�õ����ٶȴ���������											// ��ȡ MPU6050 �Ĳ�����ֵ
	Get_Angle();
}

void Yijielvbo(float angle_m, float gyro_m)
{
   angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * 0.010);  //Ҫ�ѶȽǶȺ�������5ms�ж�
}

void Get_Angle(void)
{ 
	    float Acce_Angle;
//		  if(gyrox>32768)  gyrox-=65536;                       //��������ת��  Ҳ��ͨ��shortǿ������ת��
//			if(gyroz>32768)  gyroz-=65536;                       //��������ת��
//	  	if(aacy>32768) aacy-=65536;                      //��������ת��
//		  if(aacz>32768) aacz-=65536;                      //��������ת��
			Gyro_Balance=mpu6050.Gyro.x;                                  //����ƽ����ٶ�
	   	Acce_Angle=atan2(mpu6050.Acc.y,mpu6050.Acc.z)*180/PI;                  //�������	���ɼ��ٶȼ��������ǣ���δ�˲�
			mpu6050.Gyro.x=mpu6050.Gyro.x/16.4;                                    //����������ת�� 16.4=32764/2000 +-2000Degree Per Second��Ϊ65536��
      Yijielvbo(Acce_Angle,mpu6050.Gyro.x);                         //�����˲�
	    Angle_Balance=angle;                                 //����ƽ�����
			debug_angle=(int)(Angle_Balance*100);
			Gyro_Turn=mpu6050.Gyro.z;                                     //����ת����ٶ�
			Acceleration_Z=mpu6050.Acc.z;                                 //===����Z����ٶȼ�	
}
