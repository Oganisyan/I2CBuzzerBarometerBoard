#include "MS5611Buzzer.h"
#include <math.h> 



MS5611Buzzer::MS5611Buzzer(TwoWire &wire, uint8_t addressPressure, uint8_t addressBuzzer) : 
  MS5611(wire, addressPressure),  wire_(wire), addressBuzzer_(addressBuzzer), lowpassSlow_(0.)
{
}

void MS5611Buzzer::sendData(uint8_t type, uint32_t data) {
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

uint32_t MS5611Buzzer::calcToneFreqence(uint32_t pressure) {
  if(lowpassSlow_ == 0.) {
    //Set Barriers
    lowpassFast_ = lowpassSlow_ = pressure;
    toneFreqLowpass_ = 0.;
    return 2500;
  } else {
    double toneFreq_; 
    lowpassFast_ = lowpassFast_ + (pressure - lowpassFast_) * 0.1;
    lowpassSlow_ = lowpassSlow_ + (pressure - lowpassSlow_) * 0.05;
     
    toneFreq_ = (lowpassSlow_ - lowpassFast_) * 25000;
     
    toneFreqLowpass_ = toneFreqLowpass_ + (toneFreq_ - toneFreqLowpass_) * 0.1;
 
    toneFreq_ =  constrain(toneFreqLowpass_+2500/2, 500., 8000.) ;
Serial.println((uint32_t) pressure);
    return (uint32_t) (toneFreq_);
  }
}  

void MS5611Buzzer::setEnableDbg(bool enable) {
  sendData(SEND_FLAG | SEND_DEBUG_ON, enable ? 0x00000001 : 0x00000000);
}

void MS5611Buzzer::setEnableBeep(bool enable) {
  sendData(SEND_FLAG | SEND_BEEP_ON, enable ? 0x00000001 : 0x00000000);  
}

    
void MS5611Buzzer::begin()
{
  MS5611::begin();
}

void MS5611Buzzer::loop()
{
  MS5611::loop();
  if(isReady()) {
    uint32_t toneFreq = calcToneFreqence(getPressure());
    if(toneFreq >= 500 && toneFreq <=8000) {
      //Serial.println(toneFreq);
      sendData(SEND_FLAG | SEND_FREQUENCE, toneFreq);
    }
  }
  
}
