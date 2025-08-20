#include <Igniter.h>
#include <Arduino.h>

Igniter::Igniter(uint8_t gate):
    gatePin(gate),
    firing(false){}

void Igniter::begin(){
    pinMode(gatePin, OUTPUT);
    digitalWrite(gatePin, LOW);
}

void Igniter::fire(){
    digitalWrite(gatePin, HIGH);
    firing = true;
}

void Igniter::stopFire(){
    digitalWrite(gatePin, LOW);
    firing = false;
}

bool Igniter::isFiring(){
    return firing;
}




