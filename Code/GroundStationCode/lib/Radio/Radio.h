#ifndef RADIO_H
#define RADIO_H
#include <Arduino.h>

void radioSetup(float frequency);
bool radioSend(const uint8_t *data, uint8_t len);
bool radioUpdate();
bool radioReceive(uint8_t *buf, uint8_t len);
bool radioAvailable();
int16_t getRSSI();


#endif