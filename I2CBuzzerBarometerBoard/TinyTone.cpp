#include <Arduino.h>
#include "TinyTone.h"
#include "MyEprom.h"

MyEprom myEprom;


TinyTone::TinyTone(Stream &debug, int pin) : debug_(debug), pin_(pin), beepLevel_(myEprom.getBeepLevel()), debugOn_(myEprom.isDebugOn()),
    time_(millis()+1000), freqenceUpBarrier_(myEprom.getFreqenceUpBarrier()), freqenceDownBarrier_(myEprom.getFreqenceDownBarrier()), 
    freqence_((freqenceUpBarrier_+freqenceDownBarrier_)/2)
{}

void TinyTone::begin()
{
  pinMode(pin_, OUTPUT);  
}

void TinyTone::debug(tone_update_t type, uint32_t value) {
  if(debugOn_) {
    debug_.print("TinyTone type: ");
    debug_.print(type);
    debug_.print(" value: ");
    debug_.print(value);
    debug_.print(" [ beep: ");
    debug_.print(beepLevel_);
    debug_.print(", dbg: ");
    debug_.print(debugOn_);
    debug_.print(", UpBarr: ");
    debug_.print(freqenceUpBarrier_);
    debug_.print(", DownBarr: ");
    debug_.print(freqenceDownBarrier_);
    debug_.println(" ]");
  
  }
}

void TinyTone::update(tone_update_t type, uint32_t value) {
  switch( type) {
  case FREQUENCE:
    if(value >= MIN_FREQUENCE && value <= MAX_FREQUENCE) {
      freqence_ = value;
    }
    break;  
  case UP_BARRIER:
    if(value >= MIN_FREQUENCE && value <= MAX_FREQUENCE) {
      freqenceUpBarrier_ = value;
      myEprom.setFreqenceUpBarrier(freqenceUpBarrier_);
      freqence_ = (freqenceUpBarrier_+freqenceDownBarrier_)/2;
    }
    break;
  case DOWN_BARRIER:
    if(value >= MIN_FREQUENCE && value <= MAX_FREQUENCE) {
      freqenceDownBarrier_ = value;
      myEprom.setFreqenceDownBarrier(freqenceDownBarrier_);
      freqence_ = (freqenceUpBarrier_+freqenceDownBarrier_)/2;
    }
    break;
  case BEEP_ON:
    if(value >= 0 && value <= 5) {
      beepLevel_ = value;
      myEprom.setBeepLevel(beepLevel_);
    }
    break;
  case DEBUG_ON:
    if(value == 1 || value == 0) {
      debugOn_ = (value != 0);
      myEprom.setDebugOn(debugOn_);
    }
    break;
  default: 
    break;
  }
  debug(type,value);

}



boolean TinyTone::check() {
    return (beepLevel_ != 0  && ((freqence_ >= freqenceUpBarrier_ && ddsAcc_ > 0) || freqence_ <= freqenceDownBarrier_));
}

void TinyTone::loop() {
  if(time_ < millis()) {
    time_ = millis()+25;
    ddsAcc_ += freqence_ + 2000;

    if(check())
      tone();
    else
      noTone();
  }
}

void TinyTone::tone() {
  uint32_t freqence = constrain(freqence_, MIN_FREQUENCE, MAX_FREQUENCE);
  int octave = 2;
  while(freqence > 255) {
    freqence /=2;
    octave ++; 
  }
  GTCCR = (pin_ == 4)<<COM1B0;
  TCCR1 = 1<<CTC1 | (pin_== 1) <<COM1A0  | (8-octave);  // for 1MHz clock
  OCR1C = freqence;           // set the OCR
}

void TinyTone::noTone() {
    TCCR1 = 1<<CTC1 | (pin_== 1);             // stop the counter
}
