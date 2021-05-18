// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <SoftwareSerial.h>
#include <Wire.h>

#define RX    2   // *** D3, Pin 2
#define TX    3   // *** D4, Pin 3
#define  SEND_FREQUENCE    0x00
#define  SEND_UP_BARRIER   0x01
#define  SEND_DOWN_BARRIER 0x02
#define  SEND_BEEP_ON      0x03
#define  SEND_DEBUG_ON     0x04



SoftwareSerial mySerial(RX, TX);
#define  SEND_FLAG       0x80
TwoWire wire_ = Wire;


void setup() {
  wire_.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  mySerial.begin(1200);

  // Enable debug
  sendData(SEND_DEBUG_ON, 0x00000001);
}

void sendData(uint8_t type, uint32_t data) {
    type = type | SEND_FLAG;
    char buffer[4];
    buffer[0] = (data & 0x000000FF);
    buffer[1] = (data & 0x0000FF00) >> 8;
    buffer[2] = (data & 0x00FF0000) >> 16;
    buffer[3] = (data & 0xFF000000) >> 24;

  
    wire_.beginTransmission(0x12);
    wire_.write((char)type);
    wire_.write(buffer[0]); 
    wire_.write(buffer[1]); 
    wire_.write(buffer[2]); 
    wire_.write(buffer[3]); 
    wire_.endTransmission(); 
}

void loop() {
 while (Serial.available() > 0 ) {
     String str = Serial.readString();
     Serial.println(str);
     if(str.substring(0,6).equalsIgnoreCase("-dbg=1")) {
        Serial.println("Debug ON");
        sendData(SEND_DEBUG_ON,0x00000001);
     } else if(str.substring(0,6).equalsIgnoreCase("-dbg=0")) {
        Serial.println("Debug OFF");
        sendData(SEND_DEBUG_ON,0x00000000);
     } else if(str.substring(0,7).equalsIgnoreCase("-beep=1")) {
        Serial.println("Beep ON");
        sendData(SEND_BEEP_ON,0x00000001);
     } else if(str.substring(0,7).equalsIgnoreCase("-beep=0")) {
        Serial.println("Beep OFF");
        sendData(SEND_BEEP_ON,0x00000000);
     } else {
       uint32_t freq = atoi(str.c_str());
       if(freq >= 500 && freq <= 8000) {
          Serial.print("Send frequence: ");
          Serial.println(freq);
          sendData(SEND_FREQUENCE, freq);
       } else {
          Serial.println("Unknown Command: use");
          Serial.println("\t-dbg=1|-dbg=0");
          Serial.println("\t-beep=0,1,2,3,4,5");
          Serial.println("or Frequence:"); 
          Serial.println("\tfrom 800 until 8000");
       }
     }
   }
   while (mySerial.available() > 0 ) {
      Serial.write(mySerial.read());
   }
}
