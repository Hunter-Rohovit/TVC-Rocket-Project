#include <fireIgniter.h>

void fireIgniter(int channel) {
    switch (channel) {
        case 1:
            digitalWrite(IGNITER_CH1_PIN, HIGH);
            delay(500); 
            digitalWrite(IGNITER_CH1_PIN, LOW);
            break;
        case 2:
            digitalWrite(IGNITER_CH2_PIN, HIGH);
            delay(500); 
            digitalWrite(IGNITER_CH2_PIN, LOW);
            break;
        case 3:
            digitalWrite(IGNITER_CH3_PIN, HIGH);
            delay(500);
            digitalWrite(IGNITER_CH3_PIN, LOW);
            break;
        default:
            Serial.println("Invalid channel");
    }
}