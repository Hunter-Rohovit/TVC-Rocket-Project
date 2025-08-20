#include <SD_breakout.h>

SD_breakout::SD_breakout(uint8_t csPin) : chipSelect(csPin) {}

bool SD_breakout::beginSDmodule() {
    if (!sdCard.init(SPI_HALF_SPEED, chipSelect)) {
        Serial.println("Card init failed");
        return false;
    }
    if (!volume.init(sdCard)) {
        Serial.println("Volume init failed");
        return false;
    }
    if (!root.openRoot(volume)) {
        Serial.println("Root open failed");
        return false;
    }
    return true;
}

void SD_breakout::printCardInfo() {
    Serial.println();
    Serial.print("Card type:         ");
    switch (sdCard.type()) {
        case SD_CARD_TYPE_SD1:  Serial.println("SD1"); break;
        case SD_CARD_TYPE_SD2:  Serial.println("SD2"); break;
        case SD_CARD_TYPE_SDHC: Serial.println("SDHC"); break;
        default:                Serial.println("Unknown");
    }

    Serial.print("Clusters: "); Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster: "); Serial.println(volume.blocksPerCluster());
    Serial.print("Total Blocks: "); Serial.println(volume.blocksPerCluster() * volume.clusterCount());
    Serial.println();

    uint32_t volumesize = volume.blocksPerCluster();
    volumesize *= volume.clusterCount();    
    volumesize /= 2;
    Serial.print("Volume type is: FAT"); Serial.println(volume.fatType(), DEC);
    Serial.print("Volume size (KB): "); Serial.println(volumesize);
    Serial.print("Volume size (MB): "); Serial.println(volumesize / 1024);
    Serial.print("Volume size (GB): "); Serial.println((float)volumesize / 1024.0);

    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    root.ls(LS_R | LS_DATE | LS_SIZE);
    root.rewind();
}

bool SD_breakout::writeFile(const char *filename, const char *message) {
    SdFile file;
    if (!file.open(&root, filename, O_WRITE | O_CREAT | O_APPEND)) {
        Serial.print("Error opening ");
        Serial.println(filename);
        return false;
    }

    file.println(message);
    file.close();
    Serial.println("Write done.");
    return true;
}

bool SD_breakout::readFile(const char *filename, Stream &stream) {
    SdFile file;
    if (!file.open(&root, filename, O_READ)) {
        Serial.print("Error opening ");
        Serial.println(filename);
        return false;
    }

    char c;
    while (file.read(&c, 1) == 1) {
      stream.write(c);
    }
    file.close();
    return true;
}
