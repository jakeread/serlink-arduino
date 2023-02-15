#include "serlink.h"
#include "cobs.h"

#define PIN_R 17 
#define PIN_G 16 
#define PIN_B 25 

SerLink link(&Serial);

void setup() {
  link.begin();
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_B, HIGH);
}

char echoPacket[1024];
boolean lastPinState = false;
uint32_t lastBlink = 0;

void loop() {
  if(lastBlink + 250 < millis()){
    lastBlink = millis();
    digitalWrite(PIN_R, lastPinState);
    lastPinState = !lastPinState;
  }
  link.loop();
  size_t len = link.getPacket(echoPacket, 1024);
  // stuff first 4 with decoded length, 
  echoPacket[0] = link.decodeTime << 3;
  echoPacket[1] = link.decodeTime << 2;
  echoPacket[2] = link.decodeTime << 1;
  echoPacket[3] = link.decodeTime; 
  if(len){ //} && link.clearToSend()){
    digitalWrite(PIN_G, HIGH);
    lastPinState = !lastPinState;
    link.send(echoPacket, len);
  }
}
