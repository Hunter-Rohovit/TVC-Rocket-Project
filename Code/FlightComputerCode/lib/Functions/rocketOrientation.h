#ifndef ROCKETORIENTATION_H
#define ROCKETORIENTATION_H

#include <globals.h>
#include <quaternionMath.h>

void setupIMUs();
Quat IMU1toInertial(const Quat& qIMU1);
Quat IMU2toBody(const Quat& qIMU2);
void tvcMovementTest();

#endif