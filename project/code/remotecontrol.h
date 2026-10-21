#ifndef _REMOTECONTROL_H_
#define _REMOTECONTROL_H_
#include "angle.h"
#define LOOKAHEAD_DISTANCE (0.5f) // 单位：米。请务必根据您的机器人进行调校！
typedef enum {
    MODE_MANUAL,  // 手动模式
    MODE_AUTO     // 自动模式
} ControlMode;
extern struct PID //pid参数结构体
{
    volatile double   Proportion;      //P      // 比例常数 Proportional Const
    volatile double   Integral;        //I      // 积分常数 Integral Const
    volatile double   Derivative;      //D      // 微分常数 Derivative Const
    volatile float      Error1;                 // Error[n-1]
    volatile float      Error2;                 // Error[n-2]
    volatile float      iError;                 // Error[n]
};

extern float motor1;
extern float motor2;
extern float motor3;
extern float motor0;
extern float pwm0;
extern float pwm2;
extern float x;
extern float y;
extern float lookahead_point_x;
extern float lookahead_point_y;
extern float loc_x[128];
extern float loc_y[128];
extern uint8 a;
extern uint8 collect_counter;	
extern float len;
extern float discriminant;
extern float k;
float lerp(float start, float end, float t);
float IncPIDCalc(float CurrentPoint,float SetPoint,struct PID* sptr);
float LocPIDCalc(float CurrentPoint,float SetPoint,struct PID* sptr);
void anglespeed_pid_auto();
void anglespeed_pid();
void update_control_mode(void);
void process_manual_mode(void);
void process_auto_mode(void);
void state_control(void);
void get_turn_angle(void);
#endif