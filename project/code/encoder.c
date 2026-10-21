#include "zf_common_headfile.h"
#include "encoder.h"
#include "zf_driver_encoder.h"
float encode=0;
float encode_gps=0;
void encoder_init()
{
   encoder_dir_init (TIM3_ENCOEDER,DIRECTION, PLUSE);
}
void encode_control()
{
	encode=(float)(encoder_get_count(TIM3_ENCOEDER));
	encoder_clear_count(TIM3_ENCOEDER);
}
