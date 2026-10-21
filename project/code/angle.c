#include "zf_common_headfile.h"
#include "angle.h"
#include "remotecontrol.h"
#include "encoder.h"
#include "math.h"
#include "gyroscope.h"
float acc_ratio = 2.0f;    //加速度计比例
float gyro_ratio = 1.0f;  //陀螺仪比例
float dt = 0.005f;         //采 样周期
float gyro_now=0.0f;
float last_angle=0.0f;
float roll_angle=0.0f;

float angle_z=0;
float gyro_z=0;
float acc_y=0;
float gyro_y =0;
float acc_x =0;
float acc_z =0;
float gyro_z_now=0;
float gyro_z_bias = 0;
float encoder=0;
float gyro_yaw=0;
float r_yaw=0;
float pos_x=0;
float pos_y=0;

float target_pitch_angle = 0.0f;
float pitch_angle_y=0;
static float target_pitch_rate=0;
float pwm_pitch=0;
static float pitch_angle=0;
//struct PID pid_pitch_angle = {0.05, 0.01, };
//struct PID pid_pitch_rate  = {0.05, 0.01, };

int16  i=0;
//void angle_pid()//姿态处理
//{
//		pitch_angle_y	= atan2(acc_x, acc_z) * 180.0f / M_PI; 
//    pitch_angle = 0.98 * (pitch_angle + gyro_y * dt) + (1.0f - 0.98) * pitch_angle_y;
//		target_pitch_rate = LocPIDCalc(pitch_angle, 0, &pid_pitch_angle);
//    pwm_pitch = LocPIDCalc(gyro_y, target_pitch_rate, &pid_pitch_rate);
//    if(pwm_pitch > 200) pwm_pitch = 200;
//    else if(pwm_pitch < -200) pwm_pitch = -200;
//}

//内环


void gyro_init()
{
    int i; 
    gyro_z_bias = 0.0f; 
    for(i = 0; i < 500; i++)
    {
        imu660ra_get_gyro();
        if (i >= 300) 
				{
            gyro_z_bias += imu660ra_gyro_transition(imu660ra_gyro_z);
        }
        system_delay_ms(5); 
    }
}
void get_data()
{
	imu660ra_get_gyro();
	imu660ra_get_acc();			
  gyro_data_collect();	
	gyro_z_now=imu660ra_gyro_transition(imu660ra_gyro_z);
	acc_y= imu660ra_acc_transition(imu660ra_acc_y);
  gyro_y = imu660ra_gyro_transition(imu660ra_gyro_y); // 俯仰角速度
  acc_x = imu660ra_acc_transition(imu660ra_acc_x); // 俯仰角计算需要
  acc_z = imu660ra_acc_transition(imu660ra_acc_z); // 俯仰角计算需要
	gyro_z=gyro_z_now-gyro_z_bias;
	gyro_yaw += (-gyro_z)*0.005;
	if(gyro_yaw>180)
	{
		gyro_yaw=gyro_yaw-360;
	}
  else if(gyro_yaw<-180)
	{
		gyro_yaw=gyro_yaw+360;
	}
	r_yaw=gyro_yaw*M_PI/180;
	pos_x +=  encode *sin(r_yaw)* M;
	pos_y +=  encode *cos(r_yaw)* M;
	encoder += encode*M;
	//printf(" %f , %f, %f, %d\n",lookahead_point_x,lookahead_point_y,discriminant,a);
}

float my_abs(float x)
{
	if(x<0)
		x=-x;
	return x;
}

