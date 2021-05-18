// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

#include "MS5611Buzzer.h"

#define RX    2   // *** D3, Pin 2
#define TX    3   // *** D4, Pin 3

SoftwareSerial mySerial(RX, TX);
#define  SEND_FLAG       0x80
TwoWire wire_ = Wire; // TODO TwoWire(1)
MS5611Buzzer ms5611Buzzer(wire_, 0x77, 0x12);

void setup() {
  Serial.begin(9600);
  mySerial.begin(1200);
  wire_.setClock(3400000);
  wire_.begin(/*TODO  MOG 25,26   */);
  ms5611Buzzer.begin();
}


void loop() {
  ms5611Buzzer.loop();
  while (Serial.available() > 0 ) {
     String str = Serial.readString();
     if(str.substring(0,6).equalsIgnoreCase("-dbg=1")) {
        Serial.println("Debug ON");
        ms5611Buzzer.setEnableDbg(true);
     } else if(str.substring(0,6).equalsIgnoreCase("-dbg=0")) {
        Serial.println("Debug OFF");
        ms5611Buzzer.setEnableDbg(false);
     } else if(str.substring(0,7).equalsIgnoreCase("-beep=1")) {
        Serial.println("Beep ON");
        ms5611Buzzer.setEnableBeep(true);
     } else if(str.substring(0,7).equalsIgnoreCase("-beep=0")) {
        Serial.println("Beep OFF");
        ms5611Buzzer.setEnableBeep(false);
     } else {
          Serial.println(str.substring(0,6));
          Serial.println("Unknown Command: use");
          Serial.println("\t-dbg=1|-dbg=0");
          Serial.println("\t-beep=1|-beep=0");
     }
     delay(5000);
   }
   while (mySerial.available() > 0 ) {
      Serial.write(mySerial.read());
   }

  
}
