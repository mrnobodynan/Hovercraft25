#include "zf_common_headfile.h"
#include "gps.h"
#include "encoder.h"
#include "angle.h"
#include "my_math.h"


double latitude_present;//纬度
double longitude_present; //经度
double distance_present;
double direction_present;//相对于原点的方位角
double yaw_present;//航向角
double latitude_begin;//需要auto模式下开局采集初始位置
double longitude_begin;
double ditance_axis=0;
double direction_axis=0;
double distance_aim = 0;
double direction_aim = 0;
double angle_gps = 0;
double str_gps_data_latitude[DATA_LEN];
double str_gps_data_longitude[DATA_LEN];
float str_gps_data_x[DATA_LEN]={0};
float str_gps_data_y[DATA_LEN]={0};
uint8 gps_collect_counter=0;//采集计数
uint8 gps_navigate_counter=0;//导航计数
int gps_i = 0;

void gps_data_collect(void)//后续改采集+储存
{
	if(gps_tau1201_flag&&!gps_data_parse())//0-解析成功 1-解析失败 可能数据包错误
 {
	 //stop
 }
 else
 {
	if(gps_collect_counter<DATA_LEN)
  {	
		if(state_flag2==1)
		{
				if(encoder>0.3)
				{
				encoder=0;
		    str_gps_data_latitude[gps_collect_counter] = gps_tau1201.latitude;
		    str_gps_data_longitude[gps_collect_counter] = gps_tau1201.longitude;
		    gps_collect_counter++;
		    }
		}
		else
	  {
		    if(encoder>0.3)
				{
				encoder=0;
		    str_gps_data_latitude[gps_collect_counter] = gps_tau1201.latitude;
		    str_gps_data_longitude[gps_collect_counter] = gps_tau1201.longitude;
		    gps_collect_counter++;
		    }
	  }
  }
 }
}
void gps_axis_exchange(void)//以正北为y轴，建左手坐标系，角度向右为正
{
  for(gps_i=0;gps_i<DATA_LEN;gps_i++)
	{
	ditance_axis  = get_two_points_distance(str_gps_data_latitude[0],str_gps_data_longitude[0],str_gps_data_latitude[gps_i],str_gps_data_longitude[gps_i]);
	direction_axis = get_two_points_azimuth(str_gps_data_latitude[0],str_gps_data_longitude[0],str_gps_data_latitude[gps_i],str_gps_data_longitude[gps_i]);
	direction_axis = ANGLE_TO_RAD(direction_axis);
	str_gps_data_x[gps_i] = my_sin(direction_axis) * ditance_axis;
	str_gps_data_y[gps_i] = my_cos(direction_axis) * ditance_axis;
	}
}

void gps_navigate(void)//更新当前位置（pos_x,pos_y),测量初始化时位置作为原点
{
 if(gps_tau1201_flag&&!gps_data_parse())//0-解析成功 1-解析失败 可能数据包错误
 {
	 //stop
 }
 else
 {
	latitude_present = gps_tau1201.latitude;
	longitude_present = gps_tau1201.longitude;
	yaw_present = gps_tau1201.direction;
	direction_present = get_two_points_azimuth(latitude_begin,longitude_begin,latitude_present,longitude_present);
	direction_present = ANGLE_TO_RAD(direction_present);
	distance_present = get_two_points_distance(latitude_begin,longitude_begin,latitude_present,longitude_present);
	pos_x = my_sin(direction_present) * distance_present;
	pos_y = my_cos(direction_present) * distance_present;
	
//	angle_gps = direction_aim - direction_present;
//	angle_gps = ANGLE_TO_RAD(angle_gps);
	

  }
}

