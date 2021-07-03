#include "BALANCE/balance.h"
#include "angle.h"
#include "mymath.h"
#include "motor.h"
#include "speedcontrol.h"

#define MAXPWM 800

// float ZHONGZHI=-43;//heavy block, debug block, and battary inside [MPU1]
float ZHONGZHI=-60;//heavy block, debug block, and battary inside [MPU2]


// float Balance_Kp=-60,Balance_Kd=0.05,Velocity_Kp=0.9,Velocity_Ki=0.02;//ok parameter 2021/06/23-7:14
// float Balance_Kp=-60,Balance_Kd=0.5,Velocity_Kp=0.5,Velocity_Ki=0.03;//ok parameter 2021/06/23-10:56
// float Balance_Kp=-30,Balance_Kd=0.6,Velocity_Kp=0.3,Velocity_Ki=0;//ok parameter 2021/06/23-11:13
// float Balance_Kp=-50,Balance_Kd=0.1,Velocity_Kp=0.6,Velocity_Ki=0.01;//ok parameter 2021/06/23-13:46
// float Balance_Kp=-30,Balance_Kd=0.12,Velocity_Kp=0.5,Velocity_Ki=0.007;//very ok parameter 2021/06/23-14:01
// float Balance_Kp=-40,Balance_Kd=0.1,Velocity_Kp=0.3,Velocity_Ki=0.005;//just-so-so parameter 2021/06/26-17:42
// float Balance_Kp=-80,Balance_Kd=0.4,Velocity_Kp=0.0,Velocity_Ki=0.0;//zhili fine parameter 2021/06/26-18:01
// float Balance_Kp=-48,Balance_Kd=0.24,Velocity_Kp=0.6,Velocity_Ki=0.006;//zhili and sudu fine parameter 2021/06/27-17:42
// float Balance_Kp=-40,Balance_Kd=0.24,Velocity_Kp=0.5,Velocity_Ki=0.006;//zhili and sudu fine parameter 2021/07/01-20:47
float Balance_Kp=-30,Balance_Kd=-0.72,Velocity_Kp=0.5,Velocity_Ki=0.01;//[for stand up]zhili and sudu fine parameter 2021/07/01-20:54


int motor1,motor2;

float maxueyang_speed=400;//speed
char Flag_Stop;
char Flag_Qian=0;
int Turn_Target=2;

int balance()
{  
   float Bias;
	 int Balance;
	 static int gyro;
	 gyro*=0.8;
	 gyro+=mpu6050.Gyro.y*0.2;
	 Bias=Angle_Balance-ZHONGZHI;                       
	 Balance=Balance_Kp*Bias+mpu6050.Gyro.x/*gyro*/*Balance_Kd; 
	 return Balance;
}

volatile float Encoder_Integral;
int velocity(int encoder_left,int encoder_right)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
		Movement=maxueyang_speed;
		Encoder_Least =(encoder_left+encoder_right)-0;                    
		Encoder *= 0.8;		                                                
		Encoder += Encoder_Least*0.2;	                                    
		Encoder_Integral +=Encoder;                                      
		Encoder_Integral=Encoder_Integral-Movement;                       
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;   // 3000 fine          
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;        
		if(Flag_Stop==1)   Encoder_Integral=0;      

	  return Velocity;
}

int turn(int encoder_left,int encoder_right)//ת�����?
{
	 static float Turn,Kp=-0.25,Kd=-0.04;
	 Turn=error*Kp+mpu6050.Gyro.z*Kd;
	 return Turn;
}

/*
car standing: return 1
car lying: return 0
*/
uint8_t angle_is_normal()
{
	if((angle < ZHONGZHI+20) && (angle > ZHONGZHI-20))
		return 1;
	else
		return 0;
}


void set_pwm(int pwm1,int pwm3)
{
	int pwm,pwm_turn;
	pwm=pwm1;
	pwm_turn=pwm3;

	motor1=pwm+pwm_turn;
	motor2=pwm-pwm_turn;
	
	if(motor1 > 0) 
		left_diretion=1;
	else if (motor1 < 0)
		left_diretion=2;
	if(motor2 > 0) 
		right_diretion=1;
	else if (motor2 < 0)
		right_diretion=2;
	
	motor1 = Abs(motor1);
	motor2 = Abs(motor2);
	
	
	if(motor1 > MAXPWM) motor1=MAXPWM;
	if(motor2 > MAXPWM) motor2=MAXPWM;
	
	motor_run(motor1,motor2);
}
