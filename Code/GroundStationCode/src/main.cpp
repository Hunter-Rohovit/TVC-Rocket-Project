#include <Arduino.h>
#include <Radio.h>
#include <parachuteEjectionTest.h> 

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.write("Serial port initialized\n");
  delay(1000);
  sendParachuteEjectSignal();
  
}

void loop() {
 
}
