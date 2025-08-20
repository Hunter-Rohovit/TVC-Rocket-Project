#include <quaternionMath.h>

Quat qConj(const Quat& q) {
    return { q.w, -q.x, -q.y, -q.z };
}

Quat qMul(const Quat& a, const Quat& b) {
    return {
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w
    };
}

void qNormalize(Quat& q) {
    float s = 1.0f / sqrtf(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    q.w *= s;
    q.x *= s;
    q.y *= s;
    q.z *= s;
}
