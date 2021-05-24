// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <SoftwareSerial.h>
#include <Wire.h>

#define  RX    2   // *** D3, Pin 2
#define  TX    3   // *** D4, Pin 3

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

void usage() {
    Serial.println("Unknown Command: use");
    Serial.println("\t-dbg=1|-dbg=0");
    Serial.println("\t-beep=0,1,2,3,4,5");
    Serial.println("\t[|-up=|-down=]Frequence");
    Serial.println("\tfrom 128 until 4095");
}

bool hasPrefix(const String &str, const char *prefix) {
  bool rv = str.substring(0,strlen(prefix)).equalsIgnoreCase(prefix);
  return rv;
}

uint32_t getValueForPrefix(const String &str, const char *prefix){
  uint32_t value = atoi(str.substring(strlen(prefix)).c_str());
  return value;
}

bool inRange(uint32_t value, uint32_t down, uint32_t up) {
  return (value >= down) && (value <= up); 
}

#define PRINT_AND_SEND(x,y) {Serial.print(#x);Serial.print(": ");Serial.println(y);sendData(x,y);}

void loop() {
 while (Serial.available() > 0 ) {
     String str = Serial.readString();
     str.trim();
     Serial.println(str);
     if(hasPrefix(str, "-dbg=")) {
        uint32_t value = getValueForPrefix(str, "-dbg=");
        if(inRange(value, 0, 1)) {
          PRINT_AND_SEND(SEND_DEBUG_ON,value);
        } else {
          usage();
        }
     } else if(hasPrefix(str, "-beep=")) {
        uint32_t value = getValueForPrefix(str, "-beep=");
        if(inRange(value, 0, 5)) {
          PRINT_AND_SEND(SEND_BEEP_ON,value);
        } else {
          usage();
        }
     } else if(hasPrefix(str, "-up=")) {
        uint32_t value = getValueForPrefix(str, "-up=");
        if(inRange(value, 128, 4095)) {
          PRINT_AND_SEND(SEND_UP_BARRIER,value);
        } else {
          usage();
        }
     } else if(hasPrefix(str, "-down=")) {
        uint32_t value = getValueForPrefix(str, "-down=");
        if(inRange(value, 128, 4095)) {
          PRINT_AND_SEND(SEND_DOWN_BARRIER,value);
        } else {
          usage();
        }
     } else {
        uint32_t value = getValueForPrefix(str, "");
        if(inRange(value, 128, 4095)) {
          PRINT_AND_SEND(SEND_FREQUENCE,value);
        } else {
          usage();
        }
     }     
   }
   while (mySerial.available() > 0 ) {
      Serial.write(mySerial.read());
   }
}
