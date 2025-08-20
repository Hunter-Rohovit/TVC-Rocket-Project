#include "BMP388.h"

BMP388::BMP388() {}

bool BMP388::begin(uint8_t address, TwoWire &wire) {
    if (!bmp.begin_I2C(address, &wire)) {
        return false;
    }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_50_HZ);

    initialized = true;
    return true;
}

float BMP388::readPressure() {
    return initialized && bmp.performReading() ? bmp.pressure / 100.0 : NAN; 
}

float BMP388::readTemperature() {
    return initialized && bmp.performReading() ? bmp.temperature : NAN;
}

float BMP388::readAltitude(float seaLevel_hPa) {
    return initialized && bmp.performReading() ? bmp.readAltitude(seaLevel_hPa) : NAN;
}


