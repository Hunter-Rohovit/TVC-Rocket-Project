#include <setupClampsServos.h>
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