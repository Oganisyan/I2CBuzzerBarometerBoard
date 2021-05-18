#include "MS5611.h"
#include <math.h> 




MS5611::MS5611(TwoWire &wire, uint8_t address) : kalmanFilter_(), wire_(wire), address_(address),
  callOrderTime_(millis()+500), callOrderCode_ (0x00)

{
  
  for (size_t i=0; i <6;i++)
    C[i] = 0;
}

bool MS5611::pressureIsOk(double pressure) {
  return pressure > 300. && pressure < 1250.;  
}

bool MS5611::isReady() {
 return callOrderCode_ == 0x00 && pressureIsOk(pressure_);
}
    
void MS5611::begin()
{

  wire_.beginTransmission(address_);
  wire_.write(0x1E); // reset
  wire_.endTransmission();
  delay(3000);


  for (uint8_t i=0; i<6  ; i++) {
    wire_.beginTransmission(address_);
    wire_.write(0xA2 + (i * 2));
    wire_.endTransmission();

    wire_.beginTransmission(address_);
    wire_.requestFrom((uint8_t) address_, (uint8_t) 0x06);
    delayMicroseconds(1000);
    if(wire_.available()) {
      C[i] = wire_.read() << 8 | wire_.read();
    }
  }
}

void  MS5611::orderValue(char code) {
  wire_.beginTransmission(address_);
  wire_.write(code);
  wire_.endTransmission();
  callOrderTime_ = millis() + 10;
  callOrderCode_ = code;
}

uint32_t  MS5611::getValue() 
{
  uint32_t ret = 0;
  callOrderCode_ = 0x00;
  // start read sequence
  wire_.beginTransmission(address_);
  wire_.write((char) 0x00);
  wire_.endTransmission();
  wire_.beginTransmission(address_);
  wire_.requestFrom((uint8_t)address_, (uint8_t) 0x03);
  
  ret = (wire_.available() >= 3) ? 
          (uint32_t)wire_.read() << 16 |
          (uint32_t)wire_.read() << 8  |
          (uint32_t)wire_.read() : 
          INVALID_UNIT32;
  wire_.endTransmission();
  return ret;
}

void MS5611::loop()
{
  if(callOrderTime_ > millis()) {
     return;
  }
  switch(callOrderCode_) {
    default: 
       orderValue(0x58); // Order temperature
       break;     
    case 0x58:  // wait for raw temperature
       if((rawT_= getValue()) == INVALID_UNIT32){
          orderValue(callOrderCode_); // Repeat order temperature
       } else {
          orderValue(0x48); // Order pressure
       }
       break;     
    case 0x48:          // wait for raw pressure
       if((rawP_= getValue()) == INVALID_UNIT32){
          orderValue(callOrderCode_); // Repeat order pressure
       } else {       
          scaleValues();
       }
       break;     
    }
}

void MS5611::scaleValues(){
  dT = rawT_ - ((double)C[4] * 256.);
  TEMP = ((double)dT * C[5]) / 8388608. +2000.;
  
  OFF  = (double )C[1] * 65536. + (dT * C[3]) / 128.;
  SENS = (double )C[0] * 32768. + (dT * C[2]) / 256.;
  
  
  T2 = 0.;
  OFF2 = 0.;
  SENS2 = 0.;
  if (TEMP < 2000.) {
    T2 = ((dT / 32768.) * (dT / 32768.)) / 2.;
    OFF2 = 5 * (TEMP - 2000.) * (TEMP - 2000.) / 2.;
    SENS2 = 5 * (TEMP - 2000.) * (TEMP - 2000.) / 4.;
    if (TEMP < -1500){
        OFF2 = OFF2 + 7. * (TEMP + 1500.) * (TEMP + 1500.);
        SENS2 = SENS2 + 11. * (TEMP + 1500.) * (TEMP + 1500.) / 2.;
    }
  }
  TEMP = TEMP - T2;
  OFF = OFF - OFF2;
  SENS = SENS - SENS2;
  temperature_ = (float) TEMP/ 100.;  
  
  double pressure =  (( rawP_ * SENS / 2097152 - OFF) / 32768/100);
  if(pressureIsOk(pressure)) {
    kalmanFilter_.update(pressure);
    pressure_  =  kalmanFilter_.get();  
  }
}


float MS5611::getTemperature()
{
  return temperature_;
}


long  MS5611::getPressure()
{
  return (long)(pressure_*100);
}


float MS5611::getAltitude(float qnh) {
    double A = pressure_/qnh;
    double B = 1/5.25588;
    double C = pow(A,B);
    C = 1 - C;
    C = C /0.0000225577;
    return (float)C;
}

float MS5611::getQNH(float altitude) {
    double A = altitude / 44330. +1.;
    double C = pow(A,5.25588);
    C *= pressure_;
    return (float) C;
}
