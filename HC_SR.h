#ifndef HC_SR_H
#define HC_SR_H

#include <Arduino.h>

class HCsr{
  public:
    HCsr(uint8_t sigPin) : HCsr(sigPin, sigPin) {};
    HCsr(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut = 20000UL);
    float read(uint8_t precision);

  private:
    uint8_t trig;
    uint8_t echo;
    unsigned long previousMicros;
    unsigned long timeout;
    unsigned int timing();
};

#endif