#ifndef _MY_EPROM_H_
#define _MY_EPROM_H_
#include <Arduino.h>

#define IDX_BEEP     0  // uint8_t
#define IDX_DEBUG    1  // uint8_t
#define IDX_DW_BARR  2  // uint16_t 400-8000
#define IDX_UP_BARR  4  // uint16_t 400-8000

class MyEprom {
  public:
    MyEprom();
    uint8_t  getBeepLevel();  
    void     setBeepLevel(uint8_t beepLevel);
    bool     isDebugOn();
    void     setDebugOn(bool debugOn);
    uint32_t getFreqenceDownBarrier();
    void     setFreqenceDownBarrier(uint32_t barrier);
    uint32_t getFreqenceUpBarrier();
    void     setFreqenceUpBarrier(uint32_t barrier);
};

#endif //  _MY_EPROM_H_
