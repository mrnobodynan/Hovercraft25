#ifndef _ANGLE_H
#define _ANGLE_H

#define M 0.014*3.1415926/1024*2.5
#define M_PI 3.1415926

float my_abs(float x);
float angle_calc(float angle_m, float gyro_m);
void angle_pid();

void gyro_init();
void get_data();

extern float pwm_pitch;
extern float gyro_z;
extern float gyro_z_bias ;
extern float angle_z;
extern float gyro_yaw;
extern float encoder;
extern float pos_x;
extern float pos_y;
extern uint8 a;

#endif