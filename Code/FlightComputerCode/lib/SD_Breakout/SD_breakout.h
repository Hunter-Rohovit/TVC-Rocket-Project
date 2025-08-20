#ifndef SD_BREAKOUT_H
#define SD_BREAKOUT_H

#include <SPI.h>
#include <SD.h>

class SD_breakout {
public:
    SD_breakout(uint8_t csPin);
    bool beginSDmodule();
    void printCardInfo();
    bool writeFile(const char *filename, const char *message);
    bool readFile(const char *filename, Stream &stream);
    uint8_t chipSelect;
    Sd2Card sdCard;
    SdVolume volume;
    SdFile root;
};

#endif