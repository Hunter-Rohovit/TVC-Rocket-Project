#ifndef BNO085_H
#define BNO085_H
#include <Adafruit_BNO08x.h>

class BNO085{
public:
    BNO085();
    bool begin(uint8_t address, TwoWire &wirePort);
    bool enableReports(String reportType, uint32_t frequencyHz);
    bool dataAvailable();
    sh2_SensorValue_t getSensorValues();

private:
    Adafruit_BNO08x bno;
    sh2_SensorValue_t sensorValue;     
};

#endif