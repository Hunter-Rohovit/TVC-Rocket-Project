#ifndef READGIMBAL_H
#define READGIMBAL_H

#include <globals.h>
#include <BNO085.h>
#include <QuatToEul.h>

float readGimbalAngle(const char* axis);
float readPitch();
float readYaw();

#endif