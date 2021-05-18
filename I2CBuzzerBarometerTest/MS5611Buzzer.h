#ifndef _MS5611_BUZZER_H_
#define _MS5611_BUZZER_H_
#include "MS5611.h"

#define  SEND_FLAG       0x80
#define  SEND_FREQUENCE  0x00
#define  SEND_UP_BARRIER   0x01
#define  SEND_DOWN_BARRIER 0x02
#define  SEND_BEEP_ON      0x03
#define  SEND_DEBUG_ON     0x04



class MS5611Buzzer : public MS5611
{

  
  uint8_t addressBuzzer_;
  TwoWire &wire_;

  int32_t toneFreqLowpass_; 
  double lowpassFast_; 
  double lowpassSlow_;

  void sendData(uint8_t type, uint32_t data);
  uint32_t calcToneFreqence(uint32_t pressure);
  
  public:
    MS5611Buzzer(TwoWire &wire, uint8_t addressPressure, uint8_t addressBuzzer);
    void loop();
    void begin();
    void setEnableDbg(bool enable);
    void setEnableBeep(bool enable);
};



#endif //_MS5611_BUZZER_H_
