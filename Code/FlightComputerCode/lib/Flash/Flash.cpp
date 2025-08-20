#include "FLASH.h"

FLASH::FLASH(uint8_t CS) : CSpin(CS) {}

bool FLASH::beginFLASH() {
    if (!card.init(SPI_HALF_SPEED, CSpin)) {
        return false;
    }
    if (!volume.init(card)) {
        return false;
    }
    return true;
}

void FLASH::printFlashINFO(Stream &stream) {
  root.openRoot(volume);
    stream.print("Card type:         ");
    switch (card.type()) {
        case SD_CARD_TYPE_SD1:  stream.println("SD1"); break;
        case SD_CARD_TYPE_SD2:  stream.println("SD2"); break;
        case SD_CARD_TYPE_SDHC: stream.println("SDHC"); break;
        default:                stream.println("Unknown"); break;
    }

    stream.print("Clusters:          ");
    stream.println(volume.clusterCount());
    stream.print("Blocks x Cluster:  ");
    stream.println(volume.blocksPerCluster());
    stream.print("Total Blocks:      ");
    stream.println(volume.blocksPerCluster() * volume.clusterCount());
    stream.println();

    uint32_t volumesize;
    stream.print("Volume type is:    FAT");
    stream.println(volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster();
    volumesize *= volume.clusterCount();
    volumesize /= 2; // SD card blocks are 512 bytes
    stream.print("Volume size (KB):  ");
    stream.println(volumesize);
    stream.print("Volume size (MB):  ");
    volumesize /= 1024;
    stream.println(volumesize);
    stream.print("Volume size (GB):  ");
    stream.println((float)volumesize / 1024.0);

    stream.println("\nFiles found on the card (name, date and size in bytes):");
    root.ls(LS_R | LS_DATE | LS_SIZE);
    root.close();
}

bool FLASH::writeLog(const char *filename, const char *message) {
  root.openRoot(volume);
    Serial.println("Writing...");
    SdFile file;
    if (!file.open(&root, filename, O_WRITE | O_CREAT | O_APPEND)) {
        Serial.println("Failed to open file for writing");
        return false;
    }

    file.println(message);
    file.close();
    Serial.println("Write finished");
    root.close();
    return true;
}

bool FLASH::readFile(const char *filename, Stream &stream) {
    root.openRoot(volume);
    Serial.println("Reading...");
    SdFile file;
    if (!file.open(&root, filename, O_READ)) {
        Serial.print("Failed to open ");
        Serial.println(filename);
        return false;
    }

    char c;
    while (file.read(&c, 1) == 1) {
        stream.write(c);
    }

    file.close();
    root.close();
    Serial.println("Read finished");
    return true;
}
