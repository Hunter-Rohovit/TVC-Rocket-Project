#include <waitAndPrint.h>
#include <Radio.h>
#include <Arduino.h>

void waitAndPrint(unsigned long timeoutMs = 1000) {
     Serial.println("Streaming data (press 'q' to stop)...");
    while (true) {
        if (Serial.available()) {
            char c = Serial.read();
            if (c == 'q') {
                Serial.println("Stopped streaming.");
                break;
            }
        }

        if (radioAvailable()) {
            char buffer[64];
            int len = radioReceive((uint8_t*)buffer, sizeof(buffer) - 1);
            buffer[len] = '\0';
            Serial.print("FC: ");
            Serial.println(buffer);
        }

        delay(10); 
    }
}