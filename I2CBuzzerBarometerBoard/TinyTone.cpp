#include <Arduino.h>
#include "TinyTone.h"

TinyTone::TinyTone(Stream &debug, int pin) : debug_(debug), pin_(pin), beepLevel_(1), debugOn_(true),
    time_(millis()+1000), freqenceUpBarrier_(2750), freqenceDownBarrier_(2250), freqence_((freqenceUpBarrier_+freqenceDownBarrier_)/2)
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
    debug_.println(value);
  }
}

void TinyTone::update(tone_update_t type, uint32_t value) {
  debug(type,value);
  switch( type) {
  case FREQUENCE:
    if(value >= MIN_FREQUENCE && value <= MAX_FREQUENCE) {
      freqence_ = value;
    }
    break;  
  case UP_BARRIER:
    if(value >= MIN_FREQUENCE && value <= MAX_FREQUENCE) {
      freqenceUpBarrier_ = value;
      freqence_ = (freqenceUpBarrier_+freqenceDownBarrier_)/2;
    }
    break;
  case DOWN_BARRIER:
    if(value >= MIN_FREQUENCE && value <= MAX_FREQUENCE) {
      freqenceDownBarrier_ = value;
      freqence_ = (freqenceUpBarrier_+freqenceDownBarrier_)/2;
    }
    break;
  case BEEP_ON:
    if(value >= 0 && value <= 5) {
      beepLevel_ = value;
    }
    break;
  case DEBUG_ON:
    if(value == 1 || value == 0) {
      debugOn_ = (value != 0);
    }
    break;
  default: 
    break;
  }
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
