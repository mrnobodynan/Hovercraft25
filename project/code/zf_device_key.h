
#ifndef _zf_device_key_h_
#define _zf_device_key_h_

#include "zf_common_headfile.h"

#define KEY_NUMBER      4

// 2. 定义按键按下的有效电平
#define KEY_PRESS_LEVEL  0

// 3. 定义按键功能相关的参数 (单位：毫秒)
#define KEY_MAX_SHOCK_PERIOD   20
#define KEY_LONG_PRESS_PERIOD  1000

// 按键状态枚举
typedef enum 
{
    KEY_RELEASE = 0,
    KEY_SHORT_PRESS,
    KEY_LONG_PRESS,
} key_state_enum;

// 按键索引枚举，我们将它与你的实际引脚对应起来
typedef enum 
{
    KEY_P36 = 0, // 按键1对应P3.6
    KEY_P37,     // 按键2对应P3.7
    KEY_P45,     // 按键3对应P4.5
    KEY_P46,     // 按键4对应P4.6
} key_index_enum;

// ---- 外部可调用的函数声明 ----
void key_init(unsigned long period_ms);
void key_scanner(void);
key_state_enum key_get_state(key_index_enum key_n);
void key_clear_state(key_index_enum key_n);
void key_clear_all_state(void);

#endif // __zf_device_key_h_