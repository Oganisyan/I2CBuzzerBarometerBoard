#ifndef _MS5611_H_
#define _MS5611_H_
#include "config.h"
#include "KalmanFilter.h"

#ifndef INVALID_UNIT32
#define  INVALID_UNIT32 0xFFFFFFFF
#endif


class MS5611
{
    KalmanFilter kalmanFilter_;
    TwoWire &wire_;
    uint8_t     address_;
    uint16_t C[7];
    float temperature_;
    float pressure_;
    uint32_t callOrderTime_;
    char     callOrderCode_;

    double rawP_;
    double rawT_;
    double dT;
    double TEMP;
    double OFF;
    double SENS;
    double T2;
    double OFF2;
    double SENS2;


  
    uint32_t  getValue();
    void scaleValues();
    void orderValue(char code);
    bool pressureIsOk(double pressure);

  public:
    MS5611(TwoWire &wire, uint8_t address);


    void loop();
    float getTemperature();
    long  getPressure();
    float getAltitude(float qnh=1013.25F);
    float getQNH(float altitude);
    bool isReady();
    void begin();
};



#endif //_MS5611_H_
