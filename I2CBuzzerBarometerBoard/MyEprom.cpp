#include "MyEprom.h"
#include <EEPROM.h>


MyEprom::MyEprom() {
  
}

uint8_t  MyEprom::getBeepLevel(){
   uint8_t rc = 0;
   rc = EEPROM.get(IDX_BEEP, rc);
   rc = constrain( rc, 0, 5);
   return rc;
}
  
void     MyEprom::setBeepLevel(uint8_t beepLevel){
   uint8_t value = constrain( beepLevel, 0, 5);
   EEPROM.put(IDX_BEEP, value);
}

bool     MyEprom::isDebugOn() {
   uint8_t rc = 0;
   rc = EEPROM.get(IDX_DEBUG, rc);
   return rc != 0;
}

void     MyEprom::setDebugOn(bool debugOn){
   uint8_t value = debugOn ? 1 : 0;
   EEPROM.put(IDX_DEBUG, value);
}

uint32_t MyEprom::getFreqenceUpBarrier(){
  uint16_t rv = 0;
  rv = EEPROM.get(IDX_UP_BARR, rv);
  if(rv < 400 || rv > 8000 ) rv = 2750;
  return (uint32_t) rv;
}

void     MyEprom::setFreqenceUpBarrier(uint32_t barrier){
  uint16_t rv = constrain( barrier, 400, 8000);
  EEPROM.put(IDX_UP_BARR, rv);
}

uint32_t MyEprom::getFreqenceDownBarrier(){
  uint16_t rv = 0;
  rv = EEPROM.get(IDX_DW_BARR, rv);
  if(rv < 400 || rv > 8000 ) rv = 2250;
  return (uint32_t) rv;
}

void     MyEprom::setFreqenceDownBarrier(uint32_t barrier){
  uint16_t rv = constrain( barrier, 400, 8000);
  EEPROM.put(IDX_DW_BARR, rv);
}
