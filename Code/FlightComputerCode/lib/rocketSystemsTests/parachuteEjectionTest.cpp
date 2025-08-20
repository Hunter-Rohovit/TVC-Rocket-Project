#include <parachuteEjectionTest.h>


unsigned long lastBeepTime = 0;
bool buzzerOn = false;

RFM95W radio(Radio_CS,Radio_G0,Radio_RST);
Igniter igniter(Igniter_Gate);

void receiveEjectCommand(){
    if(!radio.begin(915.0)){
        Serial.println("Radio failed to Initialize");
        setColor(0,0,255);
        while(true){};
    }
    Serial.println("Radio Initialized");
    delay(1000);
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    setColor(0,0,0);
    delay(2500);
    igniter.begin();
    Serial.println("Parachute Ready to Eject.");
    setColor(0,255,0);
    delay(2500);

    while(true){
        
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (radio.receive(buf, &len)) {
            if (len == 1 && buf[0] == ' ') {
                Serial.println("Ejecting Parachute");
                setColor(255,0,0);
                igniter.fire();
                delay(250);
                igniter.stopFire();
                setColor(0,0,0);
                break; 
            }
        }
        unsigned long now = millis();
        if (!buzzerOn && now - lastBeepTime >= 900) {
            tone(BUZZER_PIN, 1000);
            lastBeepTime = now;
            buzzerOn = true;
        } else if (buzzerOn && now - lastBeepTime >= 100) {
            noTone(BUZZER_PIN);
            lastBeepTime = now;
            buzzerOn = false;
        }
        delay(10);
    }




}