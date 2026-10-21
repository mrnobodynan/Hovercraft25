#ifndef __ENCODER_H_
#define __ENCODER_H_

#define PLUSE TIM3_ENCOEDER_P04//脉冲引脚
#define DIRECTION IO_P13//方向引脚

extern float encode;
extern float encode_gps;
void encoder_init(void);
void encode_control(void);


#endif