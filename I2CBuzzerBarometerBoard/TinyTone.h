#ifndef _TINY_TONE_H_
#define _TINY_TONE_H_
#include <Stream.h>
#include "MyEprom.h"



enum tone_update_t {
  FREQUENCE, UP_BARRIER, DOWN_BARRIER, BEEP_ON, DEBUG_ON
};

class TinyTone {
  Stream &debug_;
  uint32_t time_;
  int pin_;

  uint8_t beepLevel_;
  bool debugOn_;
  uint32_t freqenceUpBarrier_;
  uint32_t freqenceDownBarrier_;

  uint32_t freqence_;

  int16_t ddsAcc_;
  boolean check();
  void tone();
  void noTone();
  void debug(tone_update_t type, uint32_t value);

  uint8_t rawFreq_;
  uint8_t octave_;

  void setFreqence(uint32_t freqence);

public:
  TinyTone(Stream &debug, int pin);
  void begin();
  void update(tone_update_t type, uint32_t value);
  void loop();
};

#endif
