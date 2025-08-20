#ifndef GLOBALS_H
#define GLOBALS_H

#include <BNO085.h>   
#include <MG90S.h> 
#include <BMP388.h>  
#include <FLASH.h>
#include <SD_breakout.h>
#include <Arduino.h> 
#include <Wire.h> 

extern BNO085 IMU1;
extern BNO085 IMU2;
extern MG90S servo1;
extern MG90S servo2;
extern BMP388 barometer;
extern FLASH flash;
extern SD_breakout sd;

extern const float pitchUpCoeffs[7];
extern const float pitchDownCoeffs[7];
extern const float yawUpCoeffs[7];
extern const float yawDownCoeffs[7];

extern const uint8_t SERVO1_PIN;
extern const uint8_t SERVO2_PIN;

#define SD_CS_PIN 9
#define BUZZER_PIN 21
#define Radio_CS 8
#define Radio_G0 7
#define Radio_RST 6
#define Igniter_Gate 23
#define RED_PIN 22
#define GREEN_PIN 15
#define BLUE_PIN 14
#define CS_FLASH 10
#define CS_SD 9

#endif