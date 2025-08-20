#ifndef MG90S_H
#define MG90S_H
#include <Arduino.h>
#include <PWMServo.h>

class MG90S{
public:
    MG90S();
    void attach(uint8_t servo_pin);
    void setAngle(float angle);
    void setLimits(float zeroPos, float minPos, float maxPos);
    void centerHome();
    void detach();
    void setPosition(float pos);

    float zeroPosition; //offset position of the servo, position where the motor case is 0 degrees from vertical
    float minAngle; //furthest servo can rotate in the negative direction based on mechanical constraints
    float maxAngle; //furthest servo can rotate in the positive direction based on mechanical constraints
    float servoPosition; //true servo position, the actual angle being commanded to the servo motor (takes into account zero position isn't actually 0)
    float servoAngle; //different from servoPosition, the angle commanded to the computer (i.e +5 degrees or -4 degrees) 

private:
    PWMServo servo;
    uint8_t servoPin;
    
};

#endif