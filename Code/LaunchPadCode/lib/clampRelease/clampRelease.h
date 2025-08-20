#ifndef CLAMPRELEASE_H
#define CLAMPRELEASE_H

#include <PWMServo.h>
#include <Arduino.h>
#define CLAMP1_PIN 4
#define CLAMP2_PIN 5
#define CLAMP3_PIN 6
#define CLAMP4_PIN 14

PWMServo clamp1;
PWMServo clamp2;
PWMServo clamp3;
PWMServo clamp4;

const int CLAMP1_START = 0;
const int CLAMP2_START = 0;
const int CLAMP3_START = 0;
const int CLAMP4_START = 0;


void releaseClamps();

#endif