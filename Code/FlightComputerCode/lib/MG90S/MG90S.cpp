#include <MG90S.h>
#include <Arduino.h>

MG90S::MG90S(){}

void MG90S::attach(uint8_t servo_pin){
    servoPin = servo_pin;
    servo.attach(servo_pin);
}

void MG90S::detach(){
    servo.detach();
}

void MG90S::setLimits(float zeroPos, float minAng, float maxAng){
    zeroPosition = zeroPos;
    minAngle = minAng;
    maxAngle = maxAng;
}

void MG90S::centerHome(){
    pinMode(servoPin, OUTPUT);
    servo.write(zeroPosition);
    servoAngle = 0;   
    servoPosition = zeroPosition; 
}
void MG90S::setPosition(float pos){
    servo.write(pos);
    servoPosition = pos;
    servoAngle = servoPosition - zeroPosition;
}
void MG90S::setAngle(float angle){
    if(angle > maxAngle || angle< minAngle){
        Serial.println("Exceeds servo range of motion");  
    }
    else{
        servo.write(angle + zeroPosition);
        servoPosition = angle+zeroPosition;
        servoAngle = angle;
    }
}
