#include "my_math.h"

long quotient;
double x2 ;
double term ;
double result ;
double ratio ;
double angle ;
int n;


// 绝对值函数
double my_fabs(double x) reentrant{
    return (x < 0) ? -x : x;
}

// 浮点取模函数
double my_fmod(double numer, double denom) reentrant{
    if (my_fabs(denom) < EPSILON) return 0; // 避免除零
    
    // 计算整数部分
    quotient = (long)(numer / denom);
    
    // 返回余数
    return (numer - (double)quotient * denom);
}

// 正弦函数（泰勒级数展开）
double my_sin(double x) reentrant{
    // 角度归约到[-π, π]
    x = my_fmod(x, TWO_PI);
    if (x > MY_PI) x -= TWO_PI;
    if (x < -MY_PI) x += TWO_PI;
    
    // 使用三角恒等式归约到[0, π/2]
    if (x < 0) return -my_sin(-x);
    if (x > PI_2) return my_sin(MY_PI - x);
    
    // 泰勒级数展开
     x2 = x * x;
     term = x;
     result = term;
    
    for ( n = 1; n < 12; n++) {
        term *= -x2 / ((2*n) * (2*n + 1));
        result += term;
    }
    
    return result;
}

// 余弦函数（利用sin转换）
double my_cos(double x) reentrant{
    return my_sin(PI_2 - x);
}

// 反正切函数（泰勒级数展开）
double my_atan(double x) reentrant{
    if (my_fabs(x) > 1.0) {
        // 使用恒等式 atan(x) = π/2 - atan(1/x)
        return (x > 0 ? PI_2 : -PI_2) - my_atan(1.0 / x);
    }
    
    // 泰勒级数展开
     x2 = x * x;
     term = x;
     result = term;
    
    for ( n = 1; n < 20; n++) {
        term *= -x2;
        result += term / (2*n + 1);
    }
    
    return result;
}

// 双参数反正切函数
double my_atan2(double y, double x) reentrant{
    if (my_fabs(x) < EPSILON) {
        // 处理y轴上的点
        if (y > 0) return PI_2;
        if (y < 0) return -PI_2;
        return 0; // 原点
    }
    
     ratio = y / x;
     angle = my_atan(ratio);
    
    // 根据象限调整结果
    if (x < 0) {
        if (y >= 0) angle += MY_PI;
        else angle -= MY_PI;
    }
    
    return angle;
}