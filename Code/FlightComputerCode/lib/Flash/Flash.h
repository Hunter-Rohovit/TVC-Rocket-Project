#ifndef FLASH_H
#define FLASH_H

#include <SPI.h>
#include <SD.h>

class FLASH {
public:
    FLASH(uint8_t CS);
    bool beginFLASH();
    void printFlashINFO(Stream &stream = Serial);
    bool writeLog(const char *filename, const char *message);
    bool readFile(const char *filename, Stream &stream = Serial);
    uint8_t CSpin;
    Sd2Card card;
    SdVolume volume;
    SdFile root;
};

#endif