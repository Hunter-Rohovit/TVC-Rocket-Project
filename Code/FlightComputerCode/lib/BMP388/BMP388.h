#ifndef BMP388_H
#define BMP388_H
#include <Adafruit_BMP3XX.h>

class BMP388 {
public:
    BMP388();
    bool begin(uint8_t address = 0x77, TwoWire &wire = Wire);
    float readPressure();
    float readTemperature();
    float readAltitude(float seaLevel_hPa = 1013.25);

private:
    Adafruit_BMP3XX bmp;
    bool initialized = false;
};
#endif