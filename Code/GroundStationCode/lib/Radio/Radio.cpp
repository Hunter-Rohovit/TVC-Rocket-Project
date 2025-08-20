#include <RH_RF95.h>
#include <Arduino.h>

#define CS 4
#define G0 3
#define RST 2
bool sending = false;
unsigned long sendStartTime = 0;
const unsigned long sendTimeout = 100;

RH_RF95 radio(CS,G0);

void radioSetup(float frequency){
    pinMode(RST, OUTPUT);
    digitalWrite(RST, LOW);
    delay(100);
    digitalWrite(RST, HIGH);
    delay(100);

    if(!radio.init()){
        Serial.println("Radio Failed to Initialize");
    }else{
        Serial.println("Radio Initialized");
    }
     if(!radio.setFrequency(frequency)){
        Serial.println("Radio couldn't set frequency");
    }
    else{
        Serial.println("Radio Frequency Set");
    }
    radio.setTxPower(20,false);
}

bool radioSend(const uint8_t *data, uint8_t len){
    if(sending){
        return false;
    }
    if(!radio.send(data,len)){
        return false;
    }
    sending = true;
    sendStartTime = millis();
    return true;
}

bool radioUpdate(){
    if (sending && !radio.waitPacketSent(0)) {
        sending = false;
    } else if (sending && (millis() - sendStartTime > sendTimeout)) {
        sending = false; // Timeout fallback
    }
}

bool radioReceive(uint8_t *buf, uint8_t *len){
    if(radio.recv(buf,len)){
        return true;
    }
    return false;
}

bool radioAvailable(){
    return radio.available();
}
int16_t getRSSI(){
    return radio.lastRssi();
}



