#ifndef _GPS_H
#define _GPS_H
#include "gyroscope.h"

extern double angle_gps;
extern double distance_aim;
extern double latitude_begin;
extern double longitude_begin;
extern double str_gps_data_latitude[DATA_LEN];
extern double str_gps_data_longitude[DATA_LEN];
extern float str_gps_data_x[DATA_LEN];
extern float str_gps_data_y[DATA_LEN];
extern uint8 gps_collect_counter;
void gps_data_collect();
void gps_axis_exchange();
void gps_navigate();

#endif