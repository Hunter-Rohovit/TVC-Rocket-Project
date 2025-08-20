#ifndef QUATERNIONMATH_H
#define QUATERNIONMATH_H

#include <math.h>

struct Quat {
    float w;
    float x;
    float y;
    float z;
};

Quat qConj(const Quat& q);
Quat qMul(const Quat& a, const Quat& b);
void qNormalize(Quat& q);

#endif

