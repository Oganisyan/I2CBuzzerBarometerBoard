#ifndef _TINY_TONE_H_
#define _TINY_TONE_H_
#include <Stream.h>

#define MIN_FREQUENCE 500
#define MAX_FREQUENCE 8000

enum tone_update_t {
  FREQUENCE,
  UP_BARRIER,
  DOWN_BARRIER,
  BEEP_ON,
  DEBUG_ON
};

class TinyTone {
  Stream        &debug_;
  uint32_t      time_;
  int           pin_;
  int           beepLevel_;
  bool          debugOn_;

  uint32_t      freqenceUpBarrier_;
  uint32_t      freqenceDownBarrier_;
  uint32_t      freqence_; 
  
  int16_t       ddsAcc_;
  boolean       check();
  void          tone();
  void          noTone();
  void          debug(tone_update_t type,uint32_t value);

  
public:  
  TinyTone(Stream &debug, int pin);
  void begin();
  void update(tone_update_t type, uint32_t value);
  void loop();
};


#endif
