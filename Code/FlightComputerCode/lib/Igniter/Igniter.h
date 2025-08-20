#ifndef IGNITER_H
#define IGNITER_H

#include <Arduino.h>

class Igniter{
public:
    Igniter(uint8_t gate);
    void begin();
    void fire();
    void stopFire();
    bool isFiring();

private:
    uint8_t gatePin;
    bool firing;
};

#endif