
#include "zf_common_headfile.h"
#include "AI8051U.h" 
#include "zf_device_key.h"

// --- 模块内部变量 ---
static unsigned long       scanner_period_ms;
static unsigned long       key_press_time[KEY_NUMBER];
static key_state_enum      key_state[KEY_NUMBER];

// 读取按键电平的辅助函数
// 它现在直接使用 AI8051U.h 中定义的 sbit 变量
unsigned char read_key_pin_level(key_index_enum key_n)
{
    switch(key_n)
    {
        case KEY_P36: return P36;
        case KEY_P37: return P37;
        case KEY_P45: return P45;
        case KEY_P46: return P46;
        default:      return 1; // 默认返回高电平（未按下）
    }
}


// 按键初始化 (函数体无需修改)
void key_init(unsigned long period_ms)
{
    unsigned char i; 
    
    // 对于 P3 和 P4 口，通常默认为准双向口/输入模式，
    // C51程序中一般无需特殊初始化。
    // 如果你的AI8051U有特殊要求（如设置PxM0/PxM1），请在此处添加。
    
    for (i = 0; i < KEY_NUMBER; i++)
    {
        key_state[i] = KEY_RELEASE;
        key_press_time[i] = 0;
    }
    scanner_period_ms = period_ms;
}


// 按键状态扫描 (核心逻辑，函数体无需修改)
void key_scanner(void)
{
    unsigned char i;

    if (scanner_period_ms == 0) return;

    for (i = 0; i < KEY_NUMBER; i++)
    {
        if (KEY_PRESS_LEVEL == read_key_pin_level((key_index_enum)i))
        {
            key_press_time[i]++;
            if ((unsigned long)KEY_LONG_PRESS_PERIOD / scanner_period_ms <= key_press_time[i])
            {
                key_state[i] = KEY_LONG_PRESS;
            }
        }
        else 
        {
            if ((KEY_LONG_PRESS != key_state[i]) && ((unsigned long)KEY_MAX_SHOCK_PERIOD / scanner_period_ms <= key_press_time[i]))
            {
                key_state[i] = KEY_SHORT_PRESS;
            }
            else
            {
                key_state[i] = KEY_RELEASE;
            }
            key_press_time[i] = 0;
        }
    }
}

// 获取按键状态 (无需修改)
key_state_enum key_get_state(key_index_enum key_n)
{
    return key_state[key_n];
}

// 清除指定按键状态 (无需修改)
void key_clear_state(key_index_enum key_n)
{
    key_state[key_n] = KEY_RELEASE;
}

// 清除所有按键状态 (无需修改)
void key_clear_all_state(void)
{
    unsigned char i;
    for(i = 0; i < KEY_NUMBER; i++)
    {
        key_state[i] = KEY_RELEASE;
    }
}