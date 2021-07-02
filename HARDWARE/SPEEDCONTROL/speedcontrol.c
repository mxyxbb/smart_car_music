#include "include.h"
#include "speedcontrol.h"
#include "BALANCE/balance.h"

double kp=5,kd=15;
float basic_speed = 0;//��λcm/s����̬���ٶ�
float average_speed = 150;//��λcm/s���趨��Ŀ���ٶ�

int left_encoder_puslse = 0;
int right_encoder_puslse = 0;

int left_pwm = 0;
int right_pwm = 0;

u8 left_diretion = 1; //0����ֹ��1��ǰ����2������
u8 right_diretion = 1;


char str_speedcontrol[255];


float now_speed_left = 0;
float now_speed_right = 0;
int left_error = 0;
int right_error = 0;
int sum_left_speed = 0;
int sum_right_speed = 0;


int update_pwm_left(float current_speed,float target_speed)      //����PI���ٴ��룬����
{
	static int pwm_1;
  static float error_1,last_error_1,prev_error_1;  //��̬�������ڳ���ȫ���ڣ�
	static float kp_left = 3;
	static float ki_left = 1.8;
	static float kd_left = 0;
	//pwm:�������,bias:����ƫ��,last_bias:�ϴ�ƫ��,prev_bais_:���ϴ�ƫ��
  error_1=target_speed-current_speed;    //���㱾��ƫ��e(k)
  pwm_1 += (kp_left*(error_1-last_error_1)+ki_left*error_1+kd_left*(error_1-2*last_error_1+prev_error_1));   //����ʽPID������
  prev_error_1=last_error_1;  //�������ϴ�ƫ��
  last_error_1=error_1;     //������һ��ƫ��
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
	static float prevData = 0;                                 //��һ������
	static float p = 10, q = 0.003, r = 5, kGain = 0;      // q ������� r ������Ӧ�ٶ�
	static float EstimateCovariance = 0.1;//����Э����
	static float MeasureCovariance = 0.02;//����Э����
	p = p + q;
	kGain = p / ( p + r );                                      //���㿨��������
	inData = prevData + ( kGain * ( inData - prevData ) );      //���㱾���˲�����ֵ
	p = ( 1 - kGain ) * p;                                      //���²�������
	prevData = inData;
	return inData;                                             //���ع���ֵ
}

void cacl_error_1(void)
{
	char str[20];
	static double error = 0, last_error = 0;
	double adc_left_sqrt = 0;
	double adc_right_sqrt = 0;
	adc_left_sqrt = sqrt(adc_left);
	adc_right_sqrt = sqrt(adc_right);
	error = 3000*(adc_right_sqrt - adc_left_sqrt)/(adc_left + adc_right);//��ֵ��
	
	if(error<0) error = -(error*error);
	else error = error*error;
	/*-------------debug------------------*/
//	sprintf(str_speedcontrol,"%d\n\r",(int)(error*10));     //������ת��Ϊ�ַ�������
//	UART_PutStr(UART1,str_speedcontrol);
	/*---------------------------------*/
	error_processing(float_Abs(error));//�������ֶο���
	
	//error = KalmanFilter(error);
	
	//����������
	//���E�ߵ�ʱ��Ӧ������ƫ�����ּ��٣����ּ���
	//��ʱerrorΪ����
	left_error = kp*(error) + kd*(error - last_error);
	right_error = -left_error;
	last_error = error;
}

void sum_speed()
{
	//���ж������ٶ�
	//��������1024�ߣ�4��Ƶ������Ϊ30
	//����ֱ��56mm�����������Ӧ����Ϊ68
	now_speed_left = left_encoder_puslse*0.04491655*5;
	now_speed_right = right_encoder_puslse*0.04491655*5;
	//��������ʵ���ٶȣ���λcm/s
	
	if((float_Abs(left_error)>basic_speed)&&(left_error<0))
			sum_left_speed = 0;
	else sum_left_speed = float_Abs(basic_speed + left_error);
	
	if((float_Abs(right_error)>basic_speed)&&(right_error<0))
			sum_right_speed = 0;
	else sum_right_speed = float_Abs(basic_speed + right_error);
	
	//�ٶ��޷�
	if(sum_left_speed>250)	sum_left_speed = 250;
	if(sum_right_speed>250)	sum_right_speed = 250;
}

void update_pwm()
{
	left_pwm = update_pwm_left(now_speed_left,sum_left_speed);
	right_pwm = update_pwm_right(now_speed_right,sum_right_speed);
}



