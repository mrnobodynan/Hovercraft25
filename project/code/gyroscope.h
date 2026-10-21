#ifndef _GYROSCOPE_H
#define _GYROSCOPE_H

#define DATA_LEN 128

extern float angle_gyro;
extern float gyro_distance_aim;
extern float str_gyro_data_x_1[DATA_LEN];
extern float str_gyro_data_y_1[DATA_LEN];
extern float str_gyro_data_x_2[DATA_LEN];
extern float str_gyro_data_y_2[DATA_LEN];
extern float str_gyro_data_x_3[DATA_LEN];
extern float str_gyro_data_y_3[DATA_LEN];
extern float str_gyro_data_x_4[DATA_LEN];
extern float str_gyro_data_y_4[DATA_LEN];
extern uint8 gyro_collect_counter;
extern uint8 gyro_navigate_counter;
extern uint8 gyro_collect_flag;
extern uint8 state_flag1;
extern uint8 state_flag2;
extern uint8 g_request_flash_collect ;
void anjian();
void gyro_data_collect();
void gyro_navigate();


#endif