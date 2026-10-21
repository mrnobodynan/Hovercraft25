//#include "zf_common_headfile.h"
//#include "zf_device_lora3a22.h"
//#include "remotecontrol.h"
//#include "encoder.h"
//#include "angle.h"
//#include "groupone.h"
//#include "math.h"

//#define len 40

//uint16 gyro_collect_counter_one=0;
//float group_data_x_one[50];
//float group_data_y_one[50];
//float gyro_yaw_one=0;
//float gyro_z_one=0;
//float encoder=0;
//float angle_gyro=0;
//float gyro_pos_x=0;
//float gyro_pos_y=0;
//float gyro_direction_aim=0;
//void gyro_data_collect_one(void)
//{
//  if(gyro_collect_counter_one<50)
//	{		//更新航向角
//		gyro_z_one = imu660ra_gyro_transition(imu660ra_gyro_z);
//		gyro_yaw_one +=( gyro_z_one-gyro_z_bias )* 0.005;
//		if ( gyro_yaw_one > 180)
//		{
//		 gyro_yaw_one -= 360;
//		}
//		else if (gyro_yaw_one < -180)
//		{
//			gyro_yaw_one += 360;
//		}
//		gyro_yaw_one = gyro_yaw_one/180*3.1415926535898;//角度改弧度		
//		//积分位置
//		gyro_pos_x += -encode *sin(gyro_yaw_one)* 0.005;//?
//		gyro_pos_y += encode *cos(gyro_yaw_one)* 0.005;
//		encoder += encode;
//		//printf("%f,%f\n",gyro_pos_x,gyro_pos_y);
//		
//	  if(encoder>3000)
//	  {
//		  group_data_x_one[gyro_collect_counter_one] = gyro_pos_x;
//		  group_data_y_one[gyro_collect_counter_one] = gyro_pos_y;
//		  gyro_collect_counter_one++;
//			gyro_pos_x=0;
//			gyro_pos_y=0;
//		  encoder=0;
//	  }
//		printf("%f\n",gyro_collect_counter_one);
//		extern_iap_write_buff(0x0000, (uint8 *)group_data_x_one,200);
//		extern_iap_write_buff(0x0200, (uint8 *)group_data_y_one,200);
//	}
//}


//void gyro_navigate_one(void)
//{
//	if(gyro_collect_counter_one<50)
//	{
//			iap_read_buff(0x0000, (uint8 *)group_data_x_one,200);
//			iap_read_buff(0x0200, (uint8 *)group_data_y_one,200);
//			encoder += encode;
//			gyro_z_one = imu660ra_gyro_transition(imu660ra_gyro_z);
//			gyro_yaw_one +=( gyro_z_one-gyro_z_bias )* 0.005;
//			if ( gyro_yaw_one > 180)
//			{
//				gyro_yaw_one -= 360;
//			}
//			else if (gyro_yaw_one < -180)
//			{
//				gyro_yaw_one += 360;
//			}
//			gyro_yaw_one = gyro_yaw_one/180*3.1415926535898;//角度改弧
//			gyro_pos_x += -encode *sin(gyro_yaw_one)* 0.005;
//			gyro_pos_y += encode *cos(gyro_yaw_one)* 0.005;
//			if(gyro_collect_counter_one>0)
//			{
//				gyro_direction_aim = atan2((group_data_y_one[gyro_collect_counter_one]-group_data_y_one[gyro_collect_counter_one-1]),(group_data_x_one[gyro_collect_counter_one]-group_data_x_one[gyro_collect_counter_one-1]));
//			}
//			if(gyro_collect_counter_one==0) //0的情况单独讨论
//			{
//				gyro_direction_aim = atan2(group_data_y_one[gyro_collect_counter_one],group_data_x_one[gyro_collect_counter_one]);
//				if(gyro_pos_y>(group_data_y_one[0]))
//				{
//					gyro_collect_counter_one++;
//				}
//			}
//			angle_gyro = (float)(gyro_direction_aim-gyro_yaw_one)/3.14156*180;
//			if ( angle_gyro > 180)
//			{
//			  angle_gyro -= 360;
//			}
//			else if (angle_gyro < -180)
//			{
//				angle_gyro += 360;
//			}
//			if(gyro_pos_y>(group_data_y_one[gyro_collect_counter_one]-group_data_y_one[gyro_collect_counter_one-1]))
//			{
//				gyro_collect_counter_one++;
//			}
//	}
//}
//void iap_write_float(uint32 addr, float* float_arr, uint16 count) {
//    uint8* byte_buf = (uint8*)float_arr;  // 直接将float数组首地址转为字节指针
//    uint16 total_bytes = count * 4;       // 总字节数（float个数 × 4）    
//    iap_write_buff(addr, byte_buf, total_bytes);  // 一次性传入所有字节
//}


