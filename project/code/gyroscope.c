#include "user_headfile.h"
#include "zf_device_lora3a22.h"
#include "user_headfile.h"
#include "zf_device_key.h"
uint8 gyro_collect_counter = 0;
uint8 gyro_collect_flag = 0;
uint8 g_request_flash_collect = 0;
uint8 state_flag1=1;
uint8 state_flag2=0;
float str_gyro_data_x_1[DATA_LEN]={0.0};
float str_gyro_data_y_1[DATA_LEN]={0.0};
float str_gyro_data_x_2[DATA_LEN]={0.0};
float str_gyro_data_y_2[DATA_LEN]={0.0};
float str_gyro_data_x_3[DATA_LEN]={0.0};
float str_gyro_data_y_3[DATA_LEN]={0.0};
float str_gyro_data_x_4[DATA_LEN]={0.0};
float str_gyro_data_y_4[DATA_LEN]={0.0};
float encode_gyro=0;

void gyro_data_collect(void)//加记点开始按键
{
		if(gyro_collect_counter<DATA_LEN)
		{
			if(state_flag2==1)
			{
				if(encoder>0.3)
				 {
					 encoder=0;
					 str_gyro_data_x_1[gyro_collect_counter] = pos_x;
					 str_gyro_data_y_1[gyro_collect_counter] = pos_y;
					 gyro_collect_counter++;
				 }
			}
			else
			{
				if(encoder>0.3)
				 {
					 encoder=0;
					 str_gyro_data_x_1[gyro_collect_counter] = pos_x;
					 str_gyro_data_y_1[gyro_collect_counter] = pos_y;
					 gyro_collect_counter++;
				 }
			}
		}
		else if(gyro_collect_counter<DATA_LEN*2)
		{
		  if(state_flag2==1)
			{
				if(encoder>0.3)
				 {
					 encoder=0;
					 str_gyro_data_x_2[gyro_collect_counter] = pos_x;
					 str_gyro_data_y_2[gyro_collect_counter] = pos_y;
					 gyro_collect_counter++;
				 }
			}
			else
			{
				if(encoder>0.3)
				 {
					 encoder=0;
					 str_gyro_data_x_2[gyro_collect_counter] = pos_x;
					 str_gyro_data_y_2[gyro_collect_counter] = pos_y;
					 gyro_collect_counter++;
				 }
			}
		}
		else if(gyro_collect_counter<DATA_LEN*3)
		{
		  if(state_flag2==1)
			{
				if(encoder>0.3)
				 {
					 encoder=0;
					 str_gyro_data_x_3[gyro_collect_counter] = pos_x;
					 str_gyro_data_y_3[gyro_collect_counter] = pos_y;
					 gyro_collect_counter++;
				 }
			}
			else
			{
				if(encoder>0.3)
				 {
					 encoder=0;
					 str_gyro_data_x_3[gyro_collect_counter] = pos_x;
					 str_gyro_data_y_3[gyro_collect_counter] = pos_y;
					 gyro_collect_counter++;
				 }
			}
		}
		else if(gyro_collect_counter<DATA_LEN*4)
		{
		  if(state_flag2==1)
			{
				if(encoder>0.3)
				 {
					 encoder=0;
					 str_gyro_data_x_4[gyro_collect_counter] = pos_x;
					 str_gyro_data_y_4[gyro_collect_counter] = pos_y;
					 gyro_collect_counter++;
				 }
			}
			else
			{
				if(encoder>0.3)
				 {
					 encoder=0;
					 str_gyro_data_x_4[gyro_collect_counter] = pos_x;
					 str_gyro_data_y_4[gyro_collect_counter] = pos_y;
					 gyro_collect_counter++;
				 }
			}
		}
		
}
void anjian()
{
	 key_scanner();
	if (key_get_state(KEY_P36) == KEY_SHORT_PRESS)
        {
            key_clear_state(KEY_P36);       
            if (state_flag1 == 0)
            {
                state_flag1 = 1;
            }
            else 
            {
                state_flag1 = 0;
            }
						g_request_flash_collect = 1; 

        }
        if (key_get_state(KEY_P37) == KEY_SHORT_PRESS)
        {
            key_clear_state(KEY_P37); 
            state_flag2 = 1; 
        }
        if (key_get_state(KEY_P45) == KEY_SHORT_PRESS)
        {
            key_clear_state(KEY_P45);
            state_flag2 = 2; 
        }
        if (key_get_state(KEY_P46) == KEY_SHORT_PRESS)
        {
            key_clear_state(KEY_P46);
            state_flag2 = 3; 
        }
}
 