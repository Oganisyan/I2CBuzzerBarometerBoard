// Test code for Adafruit GPS modules using MTK3329/MTK3339 driver
//
// This code shows how to listen to the GPS module in an interrupt
// which allows the program to have more 'freedom' - just parse
// when a new NMEA sentence is available! Then access data when
// desired.
//
// Tested and works great with the Adafruit Ultimate GPS module
// using MTK33x9 chipset
//    ------> http://www.adafruit.com/products/746
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada

#include <TinyWireS.h>
#include <SoftwareSerial.h>
#include "TinyTone.h"

// PIN I2C_SCL              0
// PIN I2C_SDA              2 
#define DBG_TX_PIN          1 // Dbg UART pin
#define DBG_RX_PIN          3 // DBG UART pin
#define BEEP_PIN            4


// you can change the pin numbers to match your wiring:

SoftwareSerial mySerial(DBG_RX_PIN, DBG_TX_PIN);
TinyTone tinyTone(mySerial, BEEP_PIN);

#define  RCV_FLAG       0x80

#define  REQ_ERROR      0xFF

#define  RCV_FREQUENCE          0x00
#define  RCV_UP_BARRIER         0x01
#define  RCV_DOWN_BARRIER       0x02
#define  RCV_BEEP_ON            0x03
#define  RCV_DEBUG_ON           0x04

#define  INVALID_UNIT32 0xFFFFFFFF


uint8_t reqest = 0x00;

uint32_t I2C_Register[5] ={INVALID_UNIT32,INVALID_UNIT32, INVALID_UNIT32, INVALID_UNIT32, INVALID_UNIT32};


void send_data(uint8_t reqest, uint32_t data) {
  uint8_t buffer[5];
  buffer[0] = reqest; 
  buffer[1] = (data & 0x000000FF);
  buffer[2] = (data & 0x0000FF00) >> 8;
  buffer[3] = (data & 0x00FF0000) >> 16;
  buffer[4] = (data & 0xFF000000) >> 24;
  for(int i=0; i < 5; i++) {
    TinyWireS.send(buffer[i]);// 1st byte
  }
} 

uint32_t reciv_data() {
  uint8_t buffer[4];
  for(int i=0; i < 4; i++) {
     if(!TinyWireS.available())
       return 0xFFFFFFFF;
     buffer[i] = TinyWireS.receive();    
  }
  int32_t data   =    (uint32_t)buffer[3] << 24 |
                      (uint32_t)buffer[2] << 16 |
                      (uint32_t)buffer[1] << 8  |
                      (uint32_t)buffer[0];

  return data;  
} 


void requestEvent() {
  send_data(reqest, reqest < sizeof(I2C_Register) ?  I2C_Register[reqest] : INVALID_UNIT32);
}




void receiveEvent(uint8_t howMany) {
  reqest = TinyWireS.receive();
  boolean isRecv = ((reqest & RCV_FLAG) == RCV_FLAG);
  reqest = reqest & (~RCV_FLAG);
  if(reqest >= sizeof(I2C_Register)) {
      reqest = REQ_ERROR;
  } else if(isRecv) {
    I2C_Register[reqest] = reciv_data();
  }
}


void setup()
{
  tinyTone.begin();
  TinyWireS.begin(0x12);                // join i2c bus with address 0x12
  TinyWireS.onRequest(requestEvent);    // register event  
  TinyWireS.onReceive(receiveEvent);
  mySerial.begin(1200); 
}


void checkRegister(int id, tone_update_t type) {
  if(I2C_Register[id] != INVALID_UNIT32) {
    uint32_t value = I2C_Register[id];
    I2C_Register[id] = INVALID_UNIT32;
    tinyTone.update(type, value);
  }    
}

void loop()                     // run over and over again
{
  unsigned long start = millis();
  tinyTone.loop();
  TinyWireS_stop_check(); 
  checkRegister(RCV_FREQUENCE, FREQUENCE);
  checkRegister(RCV_UP_BARRIER, UP_BARRIER);
  checkRegister(RCV_DOWN_BARRIER, DOWN_BARRIER);
  checkRegister(RCV_BEEP_ON, BEEP_ON);
  checkRegister(RCV_DEBUG_ON, DEBUG_ON);
}
