#include <servoResponseLag.h>

struct StepTest {
    float from;
    float to;
};
//bunch of different changes in angles other than the standard ones already set
StepTest bigSteps[] = {
    {-22, +22}, {-20, +20}, {-15, +15}, {-10, +10}, {-5, +5},

    {-22, +18}, {-20, +15}, {-20, +10}, {-20, +5}, {-15, +20}, {-10, +22}, {-5, +20},
    {-15, +10}, {-10, +5}, {-5, +15}, {-22, 0}, {-20, 0}, {-15, 0}, {-10, 0}, {-5, 0},

    {+22, -18}, {+20, -15}, {+20, -10}, {+20, -5}, {+15, -20}, {+10, -22}, {+5, -20},
    {+15, -10}, {+10, -5}, {+5, -15}, {+22, 0}, {+20, 0}, {+15, 0}, {+10, 0}, {+5, 0},

    {-10, +5}, {-5, +10}, {+5, -10}, {+10, -5}, {-2, +2}, {+2, -2},

    {0, +22}, {0, +20}, {0, +15}, {0, +10}, {0, +5},
    {0, -22}, {0, -20}, {0, -15}, {0, -10}, {0, -5},

    {-10, +15}, {-15, +10}, {+10, -15}, {+15, -10},
    {-7, +18}, {-18, +7}, {+7, -18}, {+18, -7}
};

void logResponseLag(
    File& logfile,
    const char* axis,                
    const char* direction,          
    float currentServoAngle,
    float commandedServoAngle,
    const float* polyCoeffs,
    MG90S servo                     
) {
const float risePercent = 0.8;           
const int delayMs = 5;                   
const unsigned long timeout = 3000;   
const int preMotionDelayMs = 40;
String axis_direction;   

float expectedGimbal = evaluatePoly(axis_direction, commandedServoAngle);
float initialGimbal = readGimbalAngle(axis);
float riseTarget = initialGimbal + risePercent * (expectedGimbal - initialGimbal);
float responseLag = -1;

unsigned long t0 = millis();
bool riseFound = false;

servo.setAngle(commandedServoAngle);
delay(preMotionDelayMs);

while (millis() - t0 < timeout) {
    float actual = readGimbalAngle(axis);

    if (!isnan(actual) && actual >= riseTarget) {
        responseLag = millis() - t0;
        riseFound = true;
        break;
    }

    delay(delayMs); 
}

// If rise not found, responseLag stays at -1
if (!riseFound) {
    responseLag = -1;
}

// Log result
logfile.print(axis);                      logfile.print(",");
logfile.print(direction);                logfile.print(",");
logfile.print(currentServoAngle, 2);     logfile.print(",");
logfile.print(commandedServoAngle, 2);   logfile.print(",");
logfile.print(commandedServoAngle - currentServoAngle, 2); logfile.print(",");
logfile.println(responseLag);
}

void  findResponseLag() {
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD card initialization failed!");
        return;
    }

    File logfile = SD.open("PITREy.csv", FILE_WRITE);
    if (!logfile) {
        Serial.println("Failed to open pitch log file!");
        return;
    }

    File logfile2 = SD.open("YAWRE7.csv", FILE_WRITE);
    if (!logfile2) {
        Serial.println("Failed to open yaw log file!");
        return;
    }

    float initialPositions[] = {0, 5, 10, 15, -5, -10, -15};
    float stepSizes[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

    // CSV headers
    logfile.println("axis,direction,current_servo,commanded_servo,delta_servo,response_lag_ms");
    logfile2.println("axis,direction,current_servo,commanded_servo,delta_servo,response_lag_ms");


    delay(1000);
    homeServosToZeroPitchYaw();
    delay(1000);
    pinMode(15,OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    delay(500); 
    tone(BUZZER_PIN, 1000); // 1000 Hz tone
    delay(500);            
    noTone(BUZZER_PIN); 

    while(true){
    //Pitch
    Serial.println("Starting pitch response lag tests...");
    homeServosToZeroPitchYaw();
    delay(1000);
    Serial.println("Finished homing servos");
    delay(5000);

    for (int p = 0; p < sizeof(initialPositions) / sizeof(float); p++) {
        float initialServo = initialPositions[p];
        servo1.setAngle(initialServo);
        delay(1000);

        // UP steps
        for (int i = 0; i < sizeof(stepSizes) / sizeof(float); i++) {
            float step = stepSizes[i];
            float target = initialServo + step;
            if (abs(target) <= 22.0) {
                logResponseLag(logfile, "pitch", "up", initialServo, target, pitchUpCoeffs, servo1);
                delay(1000);
                servo1.setAngle(initialServo); // reset
                delay(1000);
            }
        }

        // DOWN steps
        for (int i = 0; i < sizeof(stepSizes) / sizeof(float); i++) {
            float step = stepSizes[i];
            float target = initialServo - step;
            if (abs(target) <= 22.0) {
                logResponseLag(logfile, "pitch", "down", initialServo, target, pitchDownCoeffs, servo1);
                delay(1000);
                servo1.setAngle(initialServo); // reset
                delay(1000);
            }
        }
    }
    Serial.println("Pitch response lag tests complete.");

    //YAW
    Serial.println("Starting yaw response lag tests...");
    homeServosToZeroPitchYaw();
    delay(1000);

    for (int p = 0; p < sizeof(initialPositions) / sizeof(float); p++) {
        float initialServo = initialPositions[p];
        servo2.setAngle(initialServo);
        delay(1000);

        // UP steps
        for (int i = 0; i < sizeof(stepSizes) / sizeof(float); i++) {
            float step = stepSizes[i];
            float target = initialServo + step;
            if (abs(target) <= 22.0) {
                logResponseLag(logfile2, "yaw", "up", initialServo, target, yawUpCoeffs, servo2);
                delay(1000);
                servo2.setAngle(initialServo); // reset
                delay(1000);
            }
        }

        // DOWN steps
        for (int i = 0; i < sizeof(stepSizes) / sizeof(float); i++) {
            float step = stepSizes[i];
            float target = initialServo - step;
            if (abs(target) <= 22.0) {
                logResponseLag(logfile2, "yaw", "down", initialServo, target, yawDownCoeffs, servo2);
                delay(1000);
                servo2.setAngle(initialServo); // reset
                delay(1000);
            }
        }
    }

    Serial.println("Starting big step transition tests...");
homeServosToZeroPitchYaw();
delay(1000);

int numTests = sizeof(bigSteps) / sizeof(StepTest);

for (int i = 0; i < numTests; i++) {
    StepTest test = bigSteps[i];

    // PITCH test
    if (abs(test.to) <= 22 && abs(test.from) <= 22) {
        servo1.setAngle(test.from);
        delay(1000);

        const char* dir = (test.to > test.from) ? "up" : "down";
        const float* coeffs = (test.to > test.from) ? pitchUpCoeffs : pitchDownCoeffs;

        logResponseLag(logfile, "pitch", dir, test.from, test.to, coeffs, servo1);
        delay(1000); 

        servo1.setAngle(test.from);  // Reset
        delay(1000);
    }

    // YAW test
    if (abs(test.to) <= 22 && abs(test.from) <= 22) {
        servo2.setAngle(test.from);
        delay(1000);

        const char* dir = (test.to > test.from) ? "up" : "down";
        const float* coeffs = (test.to > test.from) ? yawUpCoeffs : yawDownCoeffs;

        logResponseLag(logfile2, "yaw", dir, test.from, test.to, coeffs, servo2);
        delay(1000); 

        servo2.setAngle(test.from);  // Reset
        delay(1000);
    }
}

    Serial.println("Big step transition tests complete.");
    logfile.close();
    logfile2.close();
    Serial.println("Yaw response lag tests complete.");
}
}


