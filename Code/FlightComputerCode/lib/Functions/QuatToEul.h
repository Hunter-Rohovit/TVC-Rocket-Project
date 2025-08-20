#ifndef QUATTOEUL_H
#define QUATTOEUL_H

#include <Arduino.h>

struct EulerAngles {
    float roll;
    float pitch; 
    float yaw;   
};
EulerAngles convertQuatToEul(float q1, float q2, float q3, float q4);

#endif