#include <transferOnboardData.h>
#include <globals.h>

bool transferFile(const char* filename) {
    SdFile flashFile;
    SdFile sdFile;

    flash.root.openRoot(flash.volume);
    flashFile.open(&flash.root, filename, O_READ);

    sd.root.openRoot(sd.volume);
    sdFile.open(&sd.root, filename, O_WRITE | O_CREAT | O_TRUNC);

    // Transfer data using buffer
    const size_t bufSize = 64;
    uint8_t buffer[bufSize];
    int bytesRead;

    while ((bytesRead = flashFile.read(buffer, bufSize)) > 0) {
        sdFile.write(buffer, bytesRead);
    }

    flashFile.close();
    sdFile.close();

    Serial.print("Transferred file: ");
    Serial.println(filename);
    return true;
}

void transferData(const char* filenames[]){
for (int i = 0; i < 3; i++) {
    if(!transferFile(filenames[i])){
        Serial.println("failed to transfer");
    }
}


}