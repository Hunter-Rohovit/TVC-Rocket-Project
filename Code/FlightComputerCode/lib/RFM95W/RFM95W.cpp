#include <RFM95W.h>
#include <Arduino.h>

RFM95W::RFM95W(uint8_t CS, uint8_t INT, uint8_t RST):
    CS_Pin(CS),
    INT_Pin(INT),
    RST_Pin(RST),
    rfm95(CS_Pin, INT_Pin){}

bool RFM95W::begin(float frequency){
    pinMode(RST_Pin, OUTPUT);
    digitalWrite(RST_Pin, LOW);
    delay(100);
    digitalWrite(RST_Pin, HIGH);
    delay(100);

    if(!rfm95.init()){
        return false;
    }
    if(!rfm95.setFrequency(frequency)){
        return false;
    }
    rfm95.setTxPower(20, false);
    return true;
}

bool RFM95W::send(const uint8_t *data, uint8_t len){
    if(sending){
        return false;
    }
    if(!rfm95.send(data,len)){
        return false;
    }
    sending = true;
    sendStartTime = millis();
    return true;
}

void RFM95W::update(){
    if (sending && !rfm95.waitPacketSent(0)) {
        sending = false;
    } else if (sending && (millis() - sendStartTime > sendTimeout)) {
        sending = false;
    }
}

bool RFM95W::receive(uint8_t *buf, uint8_t *len){
    if(rfm95.recv(buf,len)){
        return true;
    }
    return false;
}

bool RFM95W::available(){
    return rfm95.available();
}
int16_t RFM95W::getRSSI(){
    return rfm95.lastRssi();
}

