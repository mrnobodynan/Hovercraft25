#include "zf_common_headfile.h"
#include "zf_device_lora3a22.h"
#include "remotecontrol.h"
#include "encoder.h"
#include "gyroscope.h"
#include "angle.h"
#include "math.h"
#include "zf_device_key.h"
float speed=90;
float k=2.5;
struct PID pid_gyro = {0.4,0.019,}; //角速度环//0.35,0.009  o.45,0.25
struct PID pid_v = {1.25, 0.27 , 0};//1.2,0.25


float loc_x[128]={0.0};
float loc_y[128]={0.0,5};
uint8 a=0;
uint8 m=0;
float yaw=0;
float len=0;
static ControlMode mode = MODE_MANUAL;
uint8 collect_counter=0;	
uint8 fallback_index =0;
float discriminant = 0;
float k1 = 0;
float	k2 = 0;
float lookahead_point_x=0;
float lookahead_point_y =0;
float x=0;
float y=0;
float pwm2=0;
float pwm0=0; //左边陀螺仪是正值
float motor0=0;
float motor1=0;
float motor2=0;
float motor3=0;



void anglespeed_pid()
{
	pwm2 += IncPIDCalc(gyro_z, x,  &pid_gyro);
  if(pwm2 > 350) 
		pwm2 = 350;
  else if(pwm2 < -350) 
		pwm2 = -350;
}
void anglespeed_pid_auto()
{
	pwm2 += IncPIDCalc(gyro_z,k*angle_z,  &pid_gyro);
  if(pwm2 > 350) 
		pwm2 = 350;
  else if(pwm2 < -350) 
		pwm2 = -350;
}

void process_manual_mode()
{			              
          	
			 y=(float)(lora3a22_uart_transfer.joystick[1])/2000.0f*speed;
       x=(float)(lora3a22_uart_transfer.joystick[2])/2000.0f*95.0f;	
	     anglespeed_pid();
	     if(m==20)
			 {
				  m=0;
					pwm0 +=  IncPIDCalc(encode,y,&pid_v);			
	        if(pwm0 > 350) //限值
						pwm0 = 350; 
          else	if(pwm0 < -350)		
						pwm0 = -350;
					motor3=900-pwm2/2;// - pwm_pitch;     //左前风扇,控制气垫船起落
					motor0=900+pwm2/2;// - pwm_pitch;				//右前
					motor2 = 600 + pwm0 - pwm2 ;// + pwm_pitch;	 //   左后风扇前进
					motor1 = 600 + pwm0 + pwm2 ;// + pwm_pitch;   //  右后风扇
          if(motor1 >= 950) //限值
						motor1 = 950;
					else if(motor1 <= 550)
						motor1 = 550;
					if(motor2 >= 950)
						motor2 = 950;
					else if(motor2 <= 550)
						motor2 = 550;					
          if(motor0 >= 950)
						motor0 = 950;
					if(motor3 >= 950)
						motor3 =950;
				  if((x < 5.0f && x > -5.0f) && (y < 5.0f && y > -5.0f))
					{
             pwm0 *= 0.7f; 
             if(pwm0 < 1.0f) 
							 pwm0 = 0; // 防止一直衰减
					}							
					motor1=motor1*1.01;
					if(lora3a22_uart_transfer.switch_key[2] == 1)
		       {
             pwm_set_duty(PWMB_CH2_P51,motor1);   
						 pwm_set_duty(PWMB_CH1_P50,motor0);
						 pwm_set_duty(PWMB_CH3_P52,motor2);                  
             pwm_set_duty(PWMB_CH4_P53,motor3);
					 }
					 else
					 {
						 pwm_set_duty(PWMB_CH2_P51,0);   
						 pwm_set_duty(PWMB_CH1_P50,0);
						 pwm_set_duty(PWMB_CH3_P52,0);                  
             pwm_set_duty(PWMB_CH4_P53,0);
					 }
				 }
			   m++;
}
void update_control_mode() 
{
    if (lora3a22_uart_transfer.switch_key[3] == 0) 
    {
        mode = MODE_MANUAL; 
    } 
    else if (lora3a22_uart_transfer.switch_key[3] == 1) 
    {
        mode = MODE_AUTO;
    }
}

void state_control() 
{
    switch (mode) 
    {
        case MODE_MANUAL:
            process_manual_mode();
            break;
        case MODE_AUTO:
            process_auto_mode();
            break;
        default:
            break;
    }
}


void process_auto_mode()
{
	anglespeed_pid_auto();
	if(m==20)
	{
		m=0;
	  pwm0 +=  IncPIDCalc(encode,speed,&pid_v);			
	  if(pwm0 > 350) //限值
			pwm0 = 350; 
    else	if(pwm0 < -350)		
			pwm0 = -350;
	}
	m++;
	motor3=900-pwm2/4;// - pwm_pitch;     //左前风扇,控制气垫船起落
	motor0=900+pwm2/4; //- pwm_pitch;				//右前
	motor2 = 600 + pwm0 - pwm2 ;// + pwm_pitch;	 //   左后风扇前进
	motor1 = 600 + pwm0 + pwm2 ;// + pwm_pitch;   //  右后风扇
  if(motor1 >= 950) //限值
		motor1 = 950;
	else if(motor1 <= 550)
		motor1 = 550;
	if(motor2 >= 950)
		motor2 = 950;
	else if(motor2 <= 550)
		motor2 = 550;					
  if(motor0 >= 950)
		motor0 = 950;
	if(motor3 >= 950)
		motor3 =950;
	motor1=motor1*1.01;
	if(pos_y<-0.2)
	{
		motor3 = 0;
		motor0 = 0;
		motor2 = 0;
		motor1 = 0;
	}
//	if(key_get_state(KEY_P36) == KEY_SHORT_PRESS)
//	 {
//		 pwm_set_duty(PWMB_CH2_P51,motor1);   
//		 pwm_set_duty(PWMB_CH1_P50,motor0);
//		 pwm_set_duty(PWMB_CH3_P52,motor2);                  
//		 pwm_set_duty(PWMB_CH4_P53,motor3);
//	 }	
					if(lora3a22_uart_transfer.switch_key[2] == 1)
		       {
             pwm_set_duty(PWMB_CH2_P51,motor1);   
						 pwm_set_duty(PWMB_CH1_P50,motor0);
						 pwm_set_duty(PWMB_CH3_P52,motor2);                  
             pwm_set_duty(PWMB_CH4_P53,motor3);
					 }
					 else
					 {
						 pwm_set_duty(PWMB_CH2_P51,0);   
						 pwm_set_duty(PWMB_CH1_P50,0);
						 pwm_set_duty(PWMB_CH3_P52,0);                  
             pwm_set_duty(PWMB_CH4_P53,0);
					 }
}

float IncPIDCalc(float CurrentPoint,float SetPoint,struct PID* sptr) 
{
  float	iIncpid;
  sptr->iError=SetPoint-CurrentPoint;                                     // 计算当前误差
  iIncpid=sptr->Proportion * (sptr->iError-sptr->Error1)                  // P
								+sptr->Integral * sptr->iError                                   // I
								+sptr->Derivative * (sptr->iError-2*sptr->Error1+sptr->Error2);  // D             
  sptr->Error2=sptr->Error1;                          // 存储误差，用于下次计算
  sptr->Error1=sptr->iError;												
  return(iIncpid);                                    // 返回增量值
}

float LocPIDCalc(float CurrentPoint,float SetPoint,struct PID* sptr)
{
	float iLocpid;
	sptr->Error2=SetPoint-CurrentPoint;
	sptr->iError += sptr->Error2;
//	if(sptr->iError > 100 )
//	{
//      sptr->iError = 100;
//  }
//	else if (sptr->iError < -100) 
//	{
//      sptr->iError = -100;
//  }//  if((x < 5.0f && x > -5.0f) && (sptr->Error2 < 5.0f && sptr->Error2 > -5.0f))
//  {
//     sptr->iError *= 0.7;  // 逐渐衰减而不是直接清零
//  }
	iLocpid=sptr->Proportion*sptr->Error2 + sptr->Integral*sptr->iError + sptr->Derivative*(sptr->Error2-sptr->Error1);
	sptr->Error1 = sptr->Error2;
	return(iLocpid);
}


float lerp(float start, float end, float t)
{
    return start + t * (end - start);
}

void get_turn_angle(void)
{
		uint8 i = 0;
    float x = 0, y = 0, z = 0;
	  fallback_index= ( a<collect_counter - 1) ? (a) : (collect_counter - 1);
    lookahead_point_x= loc_x[fallback_index];
		lookahead_point_y= loc_y[fallback_index];
		len = (loc_x[a] - pos_x)*(loc_x[a] - pos_x)+(loc_y[a] - pos_y)*(loc_y[a] - pos_y);
		if (len < 0.2*0.2)
		{
				a++; 
		}
    for (i = a; i < collect_counter - 1 ; i++) 
    {
        x = (loc_x[i + 1] -loc_x[i])*(loc_x[i + 1] - loc_x[i]) + (loc_y[i + 1] - loc_y[i])*(loc_y[i + 1] - loc_y[i]);
        y = 2*((loc_x[i] - pos_x) * (loc_x[i + 1] - loc_x[i]) + (loc_y[i] - pos_y)*(loc_y[i + 1] - loc_y[i]));
        z =((loc_x[i]-pos_x)*(loc_x[i]-pos_x)+(loc_y[i]-pos_y)*(loc_y[i] - pos_y)) - LOOKAHEAD_DISTANCE*LOOKAHEAD_DISTANCE;
        discriminant = y * y - 4 * x * z;
        if (discriminant >= 0) 
        {
            discriminant = (float)sqrt(discriminant); 
						k1 = (-y - discriminant) / (2 * x);
						k2 = (-y + discriminant) / (2 * x);
						if (k2 >= 0 && k2 <= 1) 
						{
								lookahead_point_x = lerp(loc_x[i], loc_x[i + 1], k2);
								lookahead_point_y = lerp(loc_y[i], loc_y[i + 1], k2);
								a = i;
								break; 
						}
						if (k1 >= 0 && k1 <= 1) 
						{
								lookahead_point_x = lerp(loc_x[i], loc_x[i + 1], k1);
								lookahead_point_y = lerp(loc_y[i], loc_y[i + 1], k1);
								a = i;
								break; 
						}
        }
    }
    yaw = atan2((lookahead_point_x - pos_x), (lookahead_point_y - pos_y));
    yaw = yaw * 180.0f / M_PI;
    angle_z = gyro_yaw - yaw;
    if (angle_z > 180.0f) 
		{
        angle_z -= 360.0f;
    }
    if (angle_z < -180.0f) 
		{
        angle_z += 360.0f;
    }
}