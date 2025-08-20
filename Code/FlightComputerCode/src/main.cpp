#include <Arduino.h>
#include <IgniterTest.h>
#include <BNO085_Print_Test.h>
#include <BMP388_Print_Test.h>
#include <Wire.h>
#include <Adafruit_BNO08x.h>
#include <FlashTest.h>
#include <LED_Test.h>
#include <SD_Test.h>
#include <Servos_test.h>
#include <radioTest.h>
#include <parachuteEjectionTest.h>
#include <servoCharacterization.h>
#include <servoResponseLag.h>
#include <servoLatency.h>


void setup(void) {
    Serial.begin(9600);
    delay(5000);
    setColor(0,255,0);
    testServos();
    

}


void loop() {
}
