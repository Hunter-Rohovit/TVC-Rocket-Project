#include "quatToEul.h"
#include <math.h>

EulerAngles convertQuatToEul(float q1, float q2, float q3, float q4) {
    EulerAngles angles;
    // Standard ZYX conversion
    float sinp = 2.0f * (q1 * q3 - q4 * q2);
    angles.pitch = asinf(sinp) * 180.0f / PI;

    float siny_cosp = 2.0f * (q1 * q4 + q2 * q3);
    float cosy_cosp = 1.0f - 2.0f * (q3 * q3 + q4 * q4);
    angles.roll = atan2f(siny_cosp, cosy_cosp) * 180.0f / PI;

    float sinr_cosp = 2.0f * (q1 * q2 + q3 * q4);
    float cosr_cosp = 1.0f - 2.0f * (q2 * q2 + q3 * q3);
    angles.yaw = atan2f(sinr_cosp, cosr_cosp) * 180.0f / PI; 

    return angles;
}