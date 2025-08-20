#include <setupIgniter.h>

void setupIgniter() {
    pinMode(IGNITER_CH1_PIN, OUTPUT);
    pinMode(IGNITER_CH2_PIN, OUTPUT);
    pinMode(IGNITER_CH3_PIN, OUTPUT);

    digitalWrite(IGNITER_CH1_PIN, LOW);
    digitalWrite(IGNITER_CH2_PIN, LOW);
    digitalWrite(IGNITER_CH3_PIN, LOW);

    delay(1000);
}