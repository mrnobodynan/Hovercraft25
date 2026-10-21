#ifndef _MY_MATH_H
#define _MY_MATH_H

#define MY_PI          3.14159265358979323846
#define PI_2        1.57079632679489661923
#define PI_4        0.78539816339744830962
#define TWO_PI      6.28318530717958647692
#define EPSILON     1e-12
extern double my_fabs (double x) reentrant;
extern double my_fmod (double numer, double denom) reentrant;
extern double my_sin  (double x) reentrant;
extern double my_cos  (double x) reentrant;
extern double my_atan (double x) reentrant;
extern double my_atan2(double y, double x) reentrant;

#endif