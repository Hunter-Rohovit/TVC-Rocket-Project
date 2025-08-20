#include <groundStation.h>
#include <Radio.h>
#include <waitAndPrint.h>

void groundStation(){
/*“s” – initialize sensors
  “h” – home TVC gimbal
  “c” – check gimbal mount orientation reading
  “b” – look at barometer readings
  “1” – look at IMU1 readings
  “2” – look at IMU2 readings
  “g” – gimbal TVC back and forth and print IMU recordings
  “L” – send command to launch (T-30 countdown, launch pad and flight computers take over)
  “[space]” – abort*/

  while(true){
        if (Serial.available()) {
        char c = Serial.read();
        if (c == 's') {
            const char* msg = "s";          
            if (radioSend((const uint8_t*)msg, strlen(msg))) {
                Serial.println("Initialize Sensors");
                waitAndPrint();
            } else {
                Serial.println("Send failed.");
            }
        }
        if (c == 'h') { 
            const char* msg = "h";          
            if (radioSend((const uint8_t*)msg, strlen(msg))) {
                Serial.println("Home TVC Mount");
                waitAndPrint();
            } else {
                Serial.println("Send failed.");
            }
        }
        if (c == 'c') { 
            const char* msg = "c";          
            if (radioSend((const uint8_t*)msg, strlen(msg))) {
                Serial.println("Check Gimbal Orientation Reading");
                waitAndPrint();
            } else {
                Serial.println("Send failed.");
            }
        }
        if (c == 'b') {
            const char* msg = "b";          
            if (radioSend((const uint8_t*)msg, strlen(msg))) {
                Serial.println("Checking Barometer Readings");
                waitAndPrint();
            } else {
                Serial.println("Send failed.");
            }
        }
        if (c == '1') {
            const char* msg = "1";          
            if (radioSend((const uint8_t*)msg, strlen(msg))) {
                Serial.println("Looking at IMU1 Readings");
                waitAndPrint();
            } else {
                Serial.println("Send failed.");
            }
        }
        if (c == '2') {
            const char* msg = "2";          
            if (radioSend((const uint8_t*)msg, strlen(msg))) {
                Serial.println("Looking at IMU2 Readings");
                waitAndPrint();
            } else {
                Serial.println("Send failed.");
            }
        }
        if (c == 'g') {
            const char* msg = " ";          
            if (radioSend((const uint8_t*)msg, strlen(msg))) {
                Serial.println("Gimbal TVC");
                waitAndPrint();
            } else {
                Serial.println("Send failed.");
            }
        }

        //Launch signal, sends a signal prior to launch command to ensure both radios are ready to receive
        //Once both radios confirm that they've recieved the signal ground station sends launch command
        if (c == 'L') {
            const char* prepMsg = "PREP";
            Serial.println("Sending PREP command to both Launch Pad and Flight Computer");
            radioSend((const uint8_t*)prepMsg, strlen(prepMsg));

            bool ackPadReceived = false;
            bool ackFcReceived = false;
            unsigned long startTime = millis();
            const unsigned long timeout = 3000; 

            // Wait for both ACKs
            while (millis() - startTime < timeout) {
                if (radioAvailable()) {
                    char buffer[32];
                    int len = radioReceive((uint8_t*)buffer, sizeof(buffer));
                    buffer[len] = '\0';

                    if (strcmp(buffer, "ACK_PAD") == 0) {
                        ackPadReceived = true;
                        Serial.println("Received ACK from Launch Pad");
                    }
                    if (strcmp(buffer, "ACK_FC") == 0) {
                        ackFcReceived = true;
                        Serial.println("Received ACK from Flight Computer");
                    }

                    if (ackPadReceived && ackFcReceived) break;
                }
            }

            if (ackPadReceived && ackFcReceived) {
                Serial.println("All ACKs received. Sending Launch Countdown Command");
                const char* beginMsg = "LAUNCH";
                radioSend((const uint8_t*)beginMsg, strlen(beginMsg));
                for (int t = 30; t >= 0; t--) {
                    Serial.print("T-");
                    Serial.println(t);
                    delay(1000);
                }

            } else {
                Serial.println("Error: One or more ACKs not received. Aborting launch sequence.");
            }
        }
        //
        if (c == ' ') {
            const char* abortMsg = "ABORT";
            Serial.println("Sending ABORT command to both devices...");
            radioSend((const uint8_t*)abortMsg, strlen(abortMsg));

            bool ackAbortPad = false;
            bool ackAbortFc = false;
            unsigned long startTime = millis();
            const unsigned long timeout = 3000;

            // Wait for both ACKs
            while (millis() - startTime < timeout) {
                if (radioAvailable()) {
                    char buffer[32];
                    int len = radioReceive((uint8_t*)buffer, sizeof(buffer));
                    buffer[len] = '\0';

                    if (strcmp(buffer, "ACK_ABORT_PAD") == 0) {
                        ackAbortPad = true;
                        Serial.println("Received ABORT ACK from Launch Pad");
                    }
                    if (strcmp(buffer, "ACK_ABORT_FC") == 0) {
                        ackAbortFc = true;
                        Serial.println("Received ABORT ACK from Flight Computer");
                    }

                    if (ackAbortPad && !ackAbortFc) break;
                }
            }

            if (ackAbortPad && ackAbortFc) {
                Serial.println("Abort confirmed by both systems.");
            } else {
                Serial.println("WARNING: Abort not confirmed by both systems!");
                if (!ackAbortPad) Serial.println("→ No ACK from Launch Pad.");
                if (!ackAbortFc) Serial.println("→ No ACK from Flight Computer.");
            }
        }


    }

    } 

}