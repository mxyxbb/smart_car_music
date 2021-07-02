#ifndef __ANGLE_H
#define __ANGLE_H	

#define PI 3.14159265

void Yijielvbo(float angle_m, float gyro_m);
void Get_Angle(void);
void angle_hanlder(void);
extern float angle;

typedef struct
{
	short x;
	short y;
	short z;
}gyro;

typedef struct
{
	short x;
	short y;
	short z;
}acc;

typedef struct
{
	gyro Gyro;
	acc Acc;
	char Acc_CALIBRATE;
	char Gyro_CALIBRATE;
}mpu;

extern mpu mpu6050;
extern float Angle_Balance,Gyro_Turn; //Æ½ºâÇã½Ç Æ½ºâÍÓÂÝÒÇ ×ªÏòÍÓÂÝÒÇ

#endif 




