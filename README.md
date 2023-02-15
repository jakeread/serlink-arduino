## USB-Serial Datalink Layer for Arduino

w/ https://github.com/qbolsee/serlink-python

## API

```
.isOpen()
.clearToSend()
.send(data, len)
.onPacket(fn(data, len)) implicit exit-flow, callback 
.getPacket(data, len) explicit exit-flow, 
```

## Packets 

### COBS-Decoded

`| n bytes | control | seq_ack | seq_tx | control key | len |`

- control is used for 
  - hello-handshake-sequence 
  - reporting window size
  - reporting max single-packet size 

## Flow Control

This is interesting, ish... we have .clearToSend() for tx-to-rx flow control, but the other end is implicit; packets leave the RX buffer whenever we call .onPacket()... so, perhaps those should be explicit user-code calls, 

## Tests

```cpp
// maxed upstream transmit... 
// could parameterize test w/ variable length paquiats 
uint8_t testPacket[128];
void loop(){
  if(serlink.clearToSend()){
    serlnk.send(testPacket, 128);
  }
}
```