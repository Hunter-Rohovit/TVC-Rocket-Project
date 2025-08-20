#include <servoLatency.h>
#include <homeServosToZeroPitchYaw.h>
#include <commandToActualTransformation.h>
#include <readGimbal.h>

void servoLatency(){
    pinMode(GREEN_PIN, OUTPUT);
    digitalWrite(GREEN_PIN, LOW);
    homeServosToZeroPitchYaw();
    delay(1000);

    for(int i = 0; i<3; i++){
        while(true){
        if (Serial.available()) {
        char ch = Serial.read();
        if (ch == ' ') {
            digitalWrite(GREEN_PIN, HIGH);
            servo2.setAngle(-22);
            delay(2000);
            digitalWrite(GREEN_PIN, LOW);
            servo2.centerHome(); 
            break;          
        }
        }
        }
    }
    for(int i = 0; i<3; i++){
        while(true){
        if (Serial.available()) {
        char ch = Serial.read();
        if (ch == ' ') {
            digitalWrite(GREEN_PIN, HIGH);
            servo2.setAngle(-15);
            delay(2000);
            digitalWrite(GREEN_PIN, LOW);
            servo2.centerHome(); 
            break;          
        }
        }
        }
    }
    for(int i = 0; i<3; i++){
       while(true){
        if (Serial.available()) {
        char ch = Serial.read();
        if (ch == ' ') {
            digitalWrite(GREEN_PIN, HIGH);
            servo2.setAngle(-10);
            delay(2000);
            digitalWrite(GREEN_PIN, LOW);
            servo2.centerHome(); 
            break;          
        }
        }
        }
    }
    for(int i = 0; i<3; i++){
        while(true){
        if (Serial.available()) {
        char ch = Serial.read();
        if (ch == ' ') {
            digitalWrite(GREEN_PIN, HIGH);
            servo2.setAngle(-5);
            delay(2000);
            digitalWrite(GREEN_PIN, LOW);
            servo2.centerHome(); 
            break;          
        }
        }
        }
    }

}

