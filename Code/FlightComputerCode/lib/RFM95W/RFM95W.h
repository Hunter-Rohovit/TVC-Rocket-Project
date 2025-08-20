#ifndef RFM95W_H
#define RFM95W_H

#include <Arduino.h>
#include <RH_RF95.h>

class RFM95W{
public:
    RFM95W(uint8_t CS, uint8_t INT, uint8_t RST);
    bool begin(float frequencyMHz = 915);
    bool send(const uint8_t *data, uint8_t len);
    void update();
    bool receive(uint8_t *buf, uint8_t *len);
    bool available();
    int16_t getRSSI();
    bool sending = false;

private:
    uint8_t CS_Pin;
    uint8_t INT_Pin;
    uint8_t RST_Pin;
    RH_RF95 rfm95;
    unsigned long sendStartTime = 0;
    const unsigned long sendTimeout = 100;
};

#endif