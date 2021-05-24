#ifndef _MY_EPROM_H_
#define _MY_EPROM_H_
#include <Arduino.h>

#define MIN_FREQUENCE 128
#define MAX_FREQUENCE 4095


#define IDX_BEEP     0  // uint8_t
#define IDX_DEBUG    1  // uint8_t
#define IDX_DW_BARR  2  // uint16_t MIN_FREQUENCE - MAX_FREQUENCE
#define IDX_UP_BARR  4  // uint16_t MIN_FREQUENCE - MAX_FREQUENCE

#define inRange(amt,low,high) ((amt < low || amt > high)  ? false: true)



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
