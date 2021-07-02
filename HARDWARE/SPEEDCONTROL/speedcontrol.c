#include "include.h"
#include "speedcontrol.h"
#include "BALANCE/balance.h"

double kp=5,kd=15;
float basic_speed = 0;//单位cm/s，动态的速度
float average_speed = 150;//单位cm/s，设定的目标速度

int left_encoder_puslse = 0;
int right_encoder_puslse = 0;

int left_pwm = 0;
int right_pwm = 0;

u8 left_diretion = 1; //0：静止，1：前进，2：后退
u8 right_diretion = 1;


char str_speedcontrol[255];


float now_speed_left = 0;
float now_speed_right = 0;
int left_error = 0;
int right_error = 0;
int sum_left_speed = 0;
int sum_right_speed = 0;


int update_pwm_left(float current_speed,float target_speed)      //增量PI调速代码，输入
{
	static int pwm_1;
  static float error_1,last_error_1,prev_error_1;  //静态变量存在程序全周期：
	static float kp_left = 3;
	static float ki_left = 1.8;
	static float kd_left = 0;
	//pwm:增量输出,bias:本次偏差,last_bias:上次偏差,prev_bais_:上上次偏差
  error_1=target_speed-current_speed;    //计算本次偏差e(k)
  pwm_1 += (kp_left*(error_1-last_error_1)+ki_left*error_1+kd_left*(error_1-2*last_error_1+prev_error_1));   //增量式PID控制器
  prev_error_1=last_error_1;  //保存上上次偏差
  last_error_1=error_1;     //保存上一次偏差
  if(pwm_1<0){
		left_diretion = 2;  
  }
	else left_diretion = 1;
  if(pwm_1>900){
    pwm_1 = 900;  
  }
  if(pwm_1<-900){
		pwm_1 = -900;
	}
 
  return pwm_1;
  
}
int update_pwm_right(float current_speed,float target_speed)    
{
	static int pwm_2;
  static float error_2,last_error_2,prev_error_2;  
	static float kp_right = 3;
	static float ki_right = 1.8;
	static float kd_right = 0;
  error_2=target_speed-current_speed;    
  pwm_2+=(kp_right*(error_2-last_error_2)+ki_right*error_2+kd_right*(error_2-2*last_error_2+prev_error_2));   
  prev_error_2=last_error_2;  
  last_error_2=error_2;      
  if(pwm_2<0){
		right_diretion = 2;
  }
	else right_diretion = 1;
  if(pwm_2>900){
    pwm_2 = 900;  
  }
	if(pwm_2<-900){
		pwm_2 = -900;
	}

  return pwm_2;
  
}

int Abs(int i)
{
	if(i>=0) return i;
	else return -i;
}
float float_Abs(float i)  
{
	if(i>=0) return i;
	else return -i;
}

float KalmanFilter(float inData)
{
	static float prevData = 0;                                 //上一个数据
	static float p = 10, q = 0.003, r = 5, kGain = 0;      // q 控制误差 r 控制响应速度
	static float EstimateCovariance = 0.1;//估计协方差
	static float MeasureCovariance = 0.02;//测量协方差
	p = p + q;
	kGain = p / ( p + r );                                      //计算卡尔曼增益
	inData = prevData + ( kGain * ( inData - prevData ) );      //计算本次滤波估计值
	p = ( 1 - kGain ) * p;                                      //更新测量方差
	prevData = inData;
	return inData;                                             //返回估计值
}

void cacl_error_1(void)
{
	char str[20];
	static double error = 0, last_error = 0;
	double adc_left_sqrt = 0;
	double adc_right_sqrt = 0;
	adc_left_sqrt = sqrt(adc_left);
	adc_right_sqrt = sqrt(adc_right);
	error = 3000*(adc_right_sqrt - adc_left_sqrt)/(adc_left + adc_right);//比值法
	
	if(error<0) error = -(error*error);
	else error = error*error;
	/*-------------debug------------------*/
//	sprintf(str_speedcontrol,"%d\n\r",(int)(error*10));     //浮点数转换为字符串函数
//	UART_PutStr(UART1,str_speedcontrol);
	/*---------------------------------*/
	error_processing(float_Abs(error));//根据误差，分段控制
	
	//error = KalmanFilter(error);
	
	//左右轮增幅
	//左边E高的时候，应该向左偏，左轮减速，右轮加速
	//此时error为负数
	left_error = kp*(error) + kd*(error - last_error);
	right_error = -left_error;
	last_error = error;
}

void sum_speed()
{
	//从中断里获得速度
	//编码器是1024线，4倍频，齿数为30
	//车轮直径56mm，与编码器对应齿数为68
	now_speed_left = left_encoder_puslse*0.04491655*5;
	now_speed_right = right_encoder_puslse*0.04491655*5;
	//两个轮子实际速度，单位cm/s
	
	if((float_Abs(left_error)>basic_speed)&&(left_error<0))
			sum_left_speed = 0;
	else sum_left_speed = float_Abs(basic_speed + left_error);
	
	if((float_Abs(right_error)>basic_speed)&&(right_error<0))
			sum_right_speed = 0;
	else sum_right_speed = float_Abs(basic_speed + right_error);
	
	//速度限幅
	if(sum_left_speed>250)	sum_left_speed = 250;
	if(sum_right_speed>250)	sum_right_speed = 250;
}

void update_pwm()
{
	left_pwm = update_pwm_left(now_speed_left,sum_left_speed);
	right_pwm = update_pwm_right(now_speed_right,sum_right_speed);
}



