#ifndef SET_CLAMPS_SERVOS_H
#define SET_CLAMPS_SERVOS_H

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


void setupClampsServos() {
    // Attach the servos to their respective pins
    clamp1.attach(CLAMP1_PIN);
    clamp2.attach(CLAMP2_PIN);
    clamp3.attach(CLAMP3_PIN);
    clamp4.attach(CLAMP4_PIN);

    // Set initial positions for the clamps
    clamp1.write(CLAMP1_START);
    clamp2.write(CLAMP2_START);
    clamp3.write(CLAMP3_START);
    clamp4.write(CLAMP4_START);

    // Allow some time for the servos to reach their positions
    delay(1000);
}

#endif
