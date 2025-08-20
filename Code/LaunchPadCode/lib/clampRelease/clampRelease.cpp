#include <clampRelease.h>

void releaseClamps(){
    clamp1.attach(CLAMP1_PIN);
    clamp2.attach(CLAMP2_PIN);
    clamp3.attach(CLAMP3_PIN);
    clamp4.attach(CLAMP4_PIN);
    delay(1000);
    
}