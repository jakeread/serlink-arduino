/*
serlink.cpp

datalink layer for usb serial objects 

Jake Read and Quentin Bolsee at the Center for Bits and Atoms
(c) Massachusetts Institute of Technology 2023

This work may be reproduced, modified, distributed, performed, and
displayed for any purpose, but must acknowledge the osap project.
Copyright is retained and must be preserved. The work is provided as is;
no warranty is provided, and users accept all liability.
*/

#include "serlink.h"
#include "cobs.h"

#define PIN_R 17 
#define PIN_G 16 
#define PIN_B 25 

SerLink::SerLink(SerialUSB* _usbcdc){
  // idk, I think nothing so far, 
  stream = _usbcdc;
}

void SerLink::begin(void){
  stream->begin(0);
}

boolean charRxBlinkState = false;
boolean packRxBlinkState = false;

// we'll run das loop here, 
void SerLink::loop(void){
  while (stream->available() && decodedLen == 0){
    // stream->write(stream->read());
    // continue;
    digitalWrite(PIN_B, charRxBlinkState);
    charRxBlinkState = !charRxBlinkState;
    // collect the next byte, 
    inBuffer[rptr ++] = stream->read();
    // do we have a packet ? 
    if(inBuffer[rptr - 1] == 0){
      digitalWrite(PIN_G, packRxBlinkState);
      packRxBlinkState = !packRxBlinkState;
      // decode it in, 
      uint32_t decodeStart = micros();
      size_t len = cobsDecode((uint8_t*)inBuffer, SERLINK_MAX_PACKET_LEN, decodedBuffer);
      decodeTime = micros() - decodeStart;
      decodedLen = len;
      rptr = 0;
      // collect the length, 
      // uint16_t reportedLength = decodedBuffer[len - 2] << 8 & decodedBuffer[len - 1];
      // // same ? 
      // if(len != reportedLength){
      //   // toss it 
      //   rptr = 0;
      // } else {
      //   // now is-full, getPacket will clear it 
      //   decodedLen = len;
      // }
    }
  } // end while-available 
}


size_t SerLink::getPacket(char* data, size_t maxLen){
  if(decodedLen > 0){
      // if(maxLen < decodedLen){
      //   // like, throw an error for a badly config'd system, 
      //   return 0;
      // } else {
      memcpy(data, decodedBuffer, decodedLen);
      size_t len = decodedLen;
      decodedLen = 0;
      return len;
    // }
  } else {
    return 0;
  }
}

void SerLink::send(char* data, size_t len){
  // first we do a little encoding, to measure, 
  uint32_t encodeStart = micros();
  size_t encLen = cobsEncode(data, len, (uint8_t*)encodedBuffer);
  encodeTime = encodeStart - micros();
  // then we stuff that in the thing, lol, 
  data[4] = encodeTime << 3;
  data[5] = encodeTime << 2;
  data[6] = encodeTime << 1;
  data[7] = encodeTime; 
  // now we have to re-encode, IIRC< 
  encLen = cobsEncode(data, len, (uint8_t*)encodedBuffer);
  // stuff the zero 
  encodedBuffer[encLen ++] = 0;
  // and we can send that... 
  // might have to do while-available... 
  for(size_t rptr = 0; rptr < encLen; rptr ++){
    stream->write(encodedBuffer[rptr]);
  }
}