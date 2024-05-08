#include "HC_SR.h"

HCsr::HCsr(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut) {
  trig = trigPin;
  echo = echoPin;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  timeout = timeOut;
}

unsigned int HCsr::timing() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  delay(30);
  previousMicros = micros();
  while(!digitalRead(echo) && (micros() - previousMicros) <= timeout);
  previousMicros = micros();
  while(digitalRead(echo) && (micros() - previousMicros) <= timeout);
  return micros() - previousMicros;
}

float HCsr::read(uint8_t precision) {
    float distance = timing() * 0.03402 / 2;
    float scalingFactor = pow(10, precision);
    distance = (int)(distance * scalingFactor + 0.5) / scalingFactor;
    return distance;
}
