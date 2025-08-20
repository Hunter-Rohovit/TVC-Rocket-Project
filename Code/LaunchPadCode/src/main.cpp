#include <Arduino.h>
#include <setupIgniter.h>
#include <setupClampsServos.h>
#include <clampRelease.h>
#include <fireIgniter.h>
#include <Radio.h>

#define LED1_R 20
#define LED1_G 19
#define LED1_B 17

#define LED2_R 23
#define LED2_G 22
#define LED2_B 21

#define BUZZER_PIN 5

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(LED1_R, OUTPUT);
  pinMode(LED1_G, OUTPUT);
  pinMode(LED1_B, OUTPUT);
  pinMode(LED2_R, OUTPUT);
  pinMode(LED2_G, OUTPUT);
  pinMode(LED2_B, OUTPUT);
  digitalWrite(LED1_R, LOW);
  digitalWrite(LED1_G, LOW);
  digitalWrite(LED1_B, LOW);
  digitalWrite(LED2_R, LOW);
  digitalWrite(LED2_G, LOW);
  digitalWrite(LED2_B, LOW);

  setupIgniter();
  digitalWrite(LED1_B, HIGH);
  delay(1000);
  setupClampsServos();
  digitalWrite(LED2_B, HIGH); 

}

void loop() {
  if (radioAvailable()) {
    char buffer[32];
    int len = radioReceive((uint8_t*)buffer, sizeof(buffer) - 1);
    buffer[len] = '\0';

    if (strcmp(buffer, "PREP") == 0) {
      Serial.println("Received PREP from Ground Station");
      const char* ack = "ACK_PAD";
      radioSend((const uint8_t*)ack, strlen(ack));
    }

    if (strcmp(buffer, "LAUNCH") == 0) {
      Serial.println("Received LAUNCH Command. Starting Countdown...");

      for (int t = 1; t >= 0; t--) {
        digitalWrite(LED1_G, HIGH);
        digitalWrite(LED2_G, HIGH);
        tone(BUZZER_PIN,1000);
        delay(100);
        digitalWrite(LED1_G, LOW);
        digitalWrite(LED2_G, LOW);
        noTone(BUZZER_PIN);
        delay(900);
      }

      Serial.println("T-0: Releasing clamps and firing igniter!");
      releaseClamps();
      fireIgniter(1); //Fire igniter on channel 1
    }

    if (strcmp(buffer, "ABORT") == 0) {
      Serial.println("Received ABORT Command. Sending ACK.");
      const char* abortAck = "ACK_ABORT_PAD";
      radioSend((const uint8_t*)abortAck, strlen(abortAck));
    }
  }
}


