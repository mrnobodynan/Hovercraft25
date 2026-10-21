
/*********************************************************************************************************************
* AI8051U Opensourec Library 即（AI8051U 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* ... (你原来的版权声明保持不变) ...
*
* 修改记录
* 日期              作者           备注
* 2024-08-01        大W            first version
* [Your Name]       [Today's Date] Refactored for stable multi-interrupt handling (C90 compatible)
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "zf_device_lora3a22.h"
#include "remotecontrol.h"
#include "encoder.h"
#include "angle.h"
#include "gps.h"
#include "zf_device_key.h"
#include "gyroscope.h"

#define PIT_CH                  (TIM0_PIT )
#define PIT_GO                  (TIM1_PIT )
#define PIT_MS					(5)

void pit_handler();
void all_control();
void out_gyro();
void display_path(float loc_x[], float loc_y[], uint8 imu_point_count);
void display_point(float current_x, float current_y);
void flash_collect_gyro();
void flash_collect_gps();
static int8 m=0;

uint16 display_start_x = 10;
uint16 display_start_y = 20;
uint16 display_width = 100;
uint16 display_height = 200;
float screen_origin_x=0;
float screen_origin_y=0;
float g_scale=0;
void main()
{
    // --- 系统初始化 ---  
		clock_init(SYSTEM_CLOCK_40M);
    debug_init();
    lora3a22_init();
    iap_init();
    encoder_init();
    ips114_init();
    imu660ra_init();
    // gps_init();
	  key_init(10);
		gyro_init();
		pit_ms_init(TIM0_PIT, 5);  
    pit_ms_init(TIM1_PIT, 5);  
	  display_path(loc_x, loc_y, 60);
	  flash_collect_gyro();
	  //flash_collect_gps();
    pwm_init(PWMB_CH2_P51, 50, 0);
    pwm_init(PWMB_CH1_P50, 50, 0);
    pwm_init(PWMB_CH3_P52, 50, 0);
    pwm_init(PWMB_CH4_P53, 50, 0);

    tim0_irq_handler = pit_handler;
    tim1_irq_handler = out_gyro;
 
    // --- 新的主循环 (任务调度器) ---
     while(1)
    {
        display_point( pos_y ,pos_x);
			  
				if(g_request_flash_collect == 1)
				{
					 flash_collect_gyro(); 
					g_request_flash_collect=0;
				}         
    }
}

void pit_handler()
{
    lora3a22_response_time++;
    if (lora3a22_response_time > 500 / PIT_MS)
    {
        lora3a22_state_flag = 0;
        lora3a22_response_time = 0;
    }
}	

void all_control()
{	
    if(lora3a22_state_flag)
    {
        if (lora3a22_finsh_flag == 1)
        {
            update_control_mode();
            state_control();
        }
        else
        {
            process_auto_mode();
        }
    }
    else
    {
         pwm_set_duty(PWMB_CH2_P51,0);   
         pwm_set_duty(PWMB_CH1_P50,0);
         pwm_set_duty(PWMB_CH3_P52,0);                  
         pwm_set_duty(PWMB_CH4_P53,0);
//			     process_auto_mode();
    }	
}

void out_gyro()//运行中断
{
	  anjian();
		encode_control();
	  get_data();
	  get_turn_angle();
		all_control();
}     

void display_path(float loc_x[], float loc_y[], uint8 imu_point_count)
{
	  uint8 i=0;
    float max_dx=0, max_dy=0;
    float dx=0, dy=0;
    max_dx = 0.0f;
    max_dy = 0.0f;
    for (i = 0; i < imu_point_count; i++)
    {
        float dx = my_abs(loc_x[i] - loc_x[0]);
        float dy = my_abs(loc_y[i] - loc_y[0]);
        if (dx > max_dx) 
					max_dx = dx;
        if (dy > max_dy) 
					max_dy = dy;
    }
    // 计算缩放比例
    g_scale = ((100 / 2.0f / max_dx) < (200 / max_dy)) ? (100 / 2.0f / max_dx) : (200 / max_dy);
}
void display_point(float current_x, float current_y)
{
    // 屏幕“原点”坐标
	  int16 px=0;
	  int16 py=0;
    screen_origin_x = 40;
    screen_origin_y = 68 ;
    // 计算当前点屏幕坐标
    py = (int16)(current_y * g_scale + screen_origin_y);
    px = (int16)(screen_origin_x +current_x  * g_scale);
    ips114_draw_point(px, py, RGB565_BLACK);
	  ips114_show_int8(4,16*1,state_flag1);
	  ips114_show_int8(4,16*2,state_flag2);
	  ips114_show_int8(4,16*3,collect_counter);
}
void flash_collect_gyro()
{
  pit_disable(TIM1_PIT);
	if(state_flag1==1)//not auto
  {  
    if(state_flag2==1)//切换科目123
		{
		iap_erase_page(0 * 512);
		iap_write_buff(0 * 512,(uint8 *)str_gyro_data_y,DATA_LEN*4);
		iap_erase_page(1 * 512);
		iap_write_buff(1 * 512,(uint8 *)str_gyro_data_x,DATA_LEN*4);
		iap_erase_page(6 * 512);
		iap_write_byte(6 * 512,gyro_collect_counter);	
		}
		else if(state_flag2==2)
		{
		iap_erase_page(2 * 512);
		iap_write_buff(2 * 512,(uint8 *)str_gyro_data_y,DATA_LEN*4);
		iap_erase_page(3 * 512);
		iap_write_buff(3 * 512,(uint8 *)str_gyro_data_x,DATA_LEN*4);
		iap_erase_page(7 * 512);
		iap_write_byte(7 * 512 ,gyro_collect_counter);	
		}
		else if(state_flag2==3)
		{
		iap_erase_page(4 * 512);
		iap_write_buff(4 * 512,(uint8 *)str_gyro_data_y,DATA_LEN*4);
		iap_erase_page(5 * 512);
		iap_write_buff(5 * 512,(uint8 *)str_gyro_data_x,DATA_LEN*4);
		iap_erase_page(8 * 512);
		iap_write_byte(8 * 512 ,gyro_collect_counter);	
		}	
  }
	if(state_flag1==0)//读点
	{
	  if(state_flag2==1)
	  {
	  iap_read_buff(0 * 512, (uint8 *)loc_y, DATA_LEN* 4);
    iap_read_buff(1 * 512, (uint8 *)loc_x, DATA_LEN* 4);
		collect_counter = iap_read_byte(6 * 512);
	  }
		else if(state_flag2==2)
		{
		iap_read_buff(2 * 512, (uint8 *)loc_y, DATA_LEN* 4);
    iap_read_buff(3 * 512, (uint8 *)loc_x, DATA_LEN* 4);
		collect_counter = iap_read_byte(7 * 512);
		}
		else if(state_flag2==3)
		{
		iap_read_buff(4 * 512, (uint8 *)loc_y, DATA_LEN* 4);
    iap_read_buff(5 * 512, (uint8 *)loc_x, DATA_LEN* 4);
		collect_counter = iap_read_byte(8 * 512);
		}
	}
	pit_enable(TIM1_PIT);
}

void flash_collect_gps()
{
  pit_disable(TIM1_PIT);
	if(state_flag1==1)//not auto
  {  
    if(state_flag2==1)//切换科目123
		{
		gps_axis_exchange();
		iap_erase_page(0 * 512);
		iap_write_buff(0 * 512,(uint8 *)str_gps_data_y,DATA_LEN*4);
		iap_erase_page(1 * 512);
		iap_write_buff(1 * 512,(uint8 *)str_gps_data_x,DATA_LEN*4);
		iap_erase_page(6 * 512);
		iap_write_byte(6 * 512,gps_collect_counter);	
		}
		else if(state_flag2==2)
		{
		gps_axis_exchange();
		iap_erase_page(2 * 512);
		iap_write_buff(2 * 512,(uint8 *)str_gps_data_y,DATA_LEN*4);
		iap_erase_page(3 * 512);
		iap_write_buff(3 * 512,(uint8 *)str_gps_data_x,DATA_LEN*4);
		iap_erase_page(7 * 512);
		iap_write_byte(7 * 512 ,gps_collect_counter);	
		}
		else if(state_flag2==3)
		{
		gps_axis_exchange();
		iap_erase_page(4 * 512);
		iap_write_buff(4 * 512,(uint8 *)str_gps_data_y,DATA_LEN*4);
		iap_erase_page(5 * 512);
		iap_write_buff(5 * 512,(uint8 *)str_gps_data_x,DATA_LEN*4);
		iap_erase_page(8 * 512);
		iap_write_byte(8 * 512 ,gps_collect_counter);	
		}	
  }
	if(state_flag1==0)//读点
	{
	  if(state_flag2==1)
	  {
	  iap_read_buff(0 * 512, (uint8 *)loc_y, DATA_LEN* 4);
    iap_read_buff(1 * 512, (uint8 *)loc_x, DATA_LEN* 4);
		collect_counter = iap_read_byte(6 * 512);
	  }
		else if(state_flag2==2)
		{
		iap_read_buff(2 * 512, (uint8 *)loc_y, DATA_LEN* 4);
    iap_read_buff(3 * 512, (uint8 *)loc_x, DATA_LEN* 4);
		collect_counter = iap_read_byte(7 * 512);
		}
		else if(state_flag2==3)
		{
		iap_read_buff(4 * 512, (uint8 *)loc_y, DATA_LEN* 4);
    iap_read_buff(5 * 512, (uint8 *)loc_x, DATA_LEN* 4);
		collect_counter = iap_read_byte(8 * 512);
		}
	}
	pit_enable(TIM1_PIT);
}
