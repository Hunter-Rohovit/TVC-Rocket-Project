#include <servoCharacterization.h>


void servoSweepTest() {
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD card initialization failed!");
        return;
    }
    Serial.println("SD card initialized.");
    File logfile = SD.open("pitch5.txt", FILE_WRITE);
    if (!logfile) {
        Serial.println("Failed to open log file!");
        return;
    }

    delay(1000);
    homeServosToZeroPitchYaw();
    delay(1000);
    pinMode(15,OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    delay(500); 
    tone(BUZZER_PIN, 1000); 
    delay(500);             
    noTone(BUZZER_PIN); 
    
    const int cycles = 50;
    const int step = 1;
    const int delayMs = 5;


    Serial.println("Starting sweep test...");

    for (int i = 0; i < cycles; i++) {
        // Sweep 0 -> +22
        for (int angle = 0; angle <= 22; angle += step) {
            servo1.setAngle(angle);
            delay(delayMs);
            float actual = readPitch();
            logfile.print(angle);
            logfile.print(", ");
            logfile.println(actual, 3);
        }

        // Sweep +22 -> -22
        for (int angle = 22; angle >= -22; angle -= step) {
            servo1.setAngle(angle);
            delay(delayMs);
            float actual = readPitch();
            logfile.print(angle);
            logfile.print(", ");
            logfile.println(actual, 3);
        }

        // Sweep -22 -> 0
        for (int angle = -22; angle <= 0; angle += step) {
            servo1.setAngle(angle);
            delay(delayMs);
            float actual = readPitch();
            logfile.print(angle);
            logfile.print(", ");
            logfile.println(actual, 3);
        }

        Serial.print("Cycle "); Serial.print(i + 1); Serial.println(" done");
    }

    logfile.close();
    Serial.println("Sweep test complete. Data saved to SD card.");

}