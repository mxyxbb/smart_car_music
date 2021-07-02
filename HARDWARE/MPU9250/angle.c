#include "angle.h"
#include <math.h>
#include <I2C_MPU9250.h>
#include "shell_port.h"

float Angle_Balance,Gyro_Balance,Gyro_Turn; //平衡倾角 平衡陀螺仪 转向陀螺仪
float Acceleration_Z;                       //Z轴加速度计  

float K1 =0.02; 
float angle;

mpu mpu6050;


int debug_angle;
SHELL_EXPORT_VAR(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_VAR_INT), a, &debug_angle, debug_angle);

void angle_hanlder (void)
{
	MPU9250_Get_Raw_data(&(mpu6050.Acc.x),&(mpu6050.Acc.y),&(mpu6050.Acc.z),&(mpu6050.Gyro.x),&(mpu6050.Gyro.y),&(mpu6050.Gyro.z));   //得到加速度传感器数据											// 获取 MPU6050 的测量数值
	Get_Angle();
}

void Yijielvbo(float angle_m, float gyro_m)
{
   angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * 0.010);  //要把度角度函数放在5ms中断
}

void Get_Angle(void)
{ 
	    float Acce_Angle;
//		  if(gyrox>32768)  gyrox-=65536;                       //数据类型转换  也可通过short强制类型转换
//			if(gyroz>32768)  gyroz-=65536;                       //数据类型转换
//	  	if(aacy>32768) aacy-=65536;                      //数据类型转换
//		  if(aacz>32768) aacz-=65536;                      //数据类型转换
			Gyro_Balance=mpu6050.Gyro.x;                                  //更新平衡角速度
	   	Acce_Angle=atan2(mpu6050.Acc.y,mpu6050.Acc.z)*180/PI;                  //计算倾角	，由加速度计算绝对倾角，尚未滤波
			mpu6050.Gyro.x=mpu6050.Gyro.x/16.4;                                    //陀螺仪量程转换 16.4=32764/2000 +-2000Degree Per Second分为65536份
      Yijielvbo(Acce_Angle,mpu6050.Gyro.x);                         //互补滤波
	    Angle_Balance=angle;                                 //更新平衡倾角
			debug_angle=(int)(Angle_Balance*100);
			Gyro_Turn=mpu6050.Gyro.z;                                     //更新转向角速度
			Acceleration_Z=mpu6050.Acc.z;                                 //===更新Z轴加速度计	
}
