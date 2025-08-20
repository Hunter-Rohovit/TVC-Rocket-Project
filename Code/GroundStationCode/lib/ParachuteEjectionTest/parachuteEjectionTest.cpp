#include <Radio.h>
#include <Arduino.h>

void sendParachuteEjectSignal(){
    radioSetup(915.0);
    delay(1000);
    Serial.println("Press Space to Eject Parachute");

    while(true){
        if (Serial.available()) {
        char c = Serial.read();
        if (c == ' ') { 
            const char* msg = " ";
            if (radioSend((const uint8_t*)msg, strlen(msg))) {
                Serial.println("Ejecting Parachute");
            } else {
                Serial.println("Send failed.");
            }
        }
    }

    }

}