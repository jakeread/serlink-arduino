/*
serlink.h

datalink layer for usb serial objects 

Jake Read and Quentin Bolsee at the Center for Bits and Atoms
(c) Massachusetts Institute of Technology 2023

This work may be reproduced, modified, distributed, performed, and
displayed for any purpose, but must acknowledge the osap project.
Copyright is retained and must be preserved. The work is provided as is;
no warranty is provided, and users accept all liability.
*/

#include <Arduino.h>

// we could / should template the below, 
#define SERLINK_MAX_PACKET_LEN 1024

class SerLink {
  public:
    // test objects, 
    uint32_t decodeTime = 0;
    uint32_t encodeTime = 0;
    // construct 
    SerLink(SerialUSB* _usbcdc);
    void begin(void);
    void loop(void);
    // rx.. 
    size_t getPacket(char* data, size_t maxLen);
    boolean isOpen(void);
    // tx.. 
    void send(char* data, size_t len);
    // boolean clearToSend(void);
  private:
    SerialUSB* stream;
    size_t rptr = 0;
    char inBuffer[SERLINK_MAX_PACKET_LEN];
    size_t decodedLen = 0;
    char decodedBuffer[SERLINK_MAX_PACKET_LEN];
    char encodedBuffer[SERLINK_MAX_PACKET_LEN];
};