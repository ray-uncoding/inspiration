#include <Adafruit_NeoPixel.h>
#include "HC_SR.h"
#define MONITOR_BAUDRATE 921600

#define LED_PIN 11
#define NUM_UNITS 4                                                     // 單元數量
#define NUM_LEDS_PER_UNIT 64                                            // 每個單元的LED數量
#define NUM_LEDS_TOTAL (NUM_UNITS * NUM_LEDS_PER_UNIT)                  // 總LED數量
Adafruit_NeoPixel leds(NUM_LEDS_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800);  //  定義ws2812燈條
float client_RGB[3] = {128.50, 255.50, 74.22};

unsigned long previousMillis = 0;
const int interval = 50;

/*
  B(7, 0)   D(0, 0)
  A(7, 7)   C(0, 7)
*/
int center_X_Y[4][2] = { //ABCD
  { 7, 7 }, { 7, 0 }, 
  { 0, 7 }, { 0, 0 } 
};

HCsr HCsr_x(12, 13);
HCsr HCsr_y(10, 11);
HCsr HCsr_z(8, 9);

float sensor_x = 0;
float sensor_y = 0;
float L = 8; 

void setup() {
  Serial.begin(MONITOR_BAUDRATE);
  leds.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    map_function();
    leds.show();
  }
}

void map_function() {

  float sensor_deta[3];   //每個感測器感測到的公分距離傳換成LED MAP的座標
  sensor_deta[0] = (L - HCsr_x.read(2)) * 8 / L;
  sensor_deta[1] = (L - HCsr_y.read(2)) * 8 / L;
  sensor_deta[2] = (L - HCsr_z.read(2)) * 8 / L;

  for(int i = 0; i<3; i++){
    sensor_deta[i] = constrain(sensor_deta[i], -L, L);
  }

  for (int unit = 0; unit < NUM_UNITS; unit++) {
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {

        //當前分析位置在LED MAP的座標
        float relativeX = x - center_X_Y[unit][0];
        float relativeY = y - center_X_Y[unit][1];

        //LED map上的光源強度公式
        float X_Y_distance = distance_calculate(relativeX, relativeY, sensor_deta[0], sensor_deta[1]);
        float a = map(sensor_deta[2] , -L, L, 0.8, 0.5);
        float bright = exp(- X_Y_distance * a);
        bright = constrain(bright, 0.00, 1.0);

        leds.setPixelColor((x + y * 8 + unit * 64), bright * client_RGB[0], bright * client_RGB[1], bright * client_RGB[2]);
      }
    }
  }
}

float distance_calculate(float x, float y, float Ox, float Oy){
  return sqrt(sq(x - Ox) + sq(y - Oy));
}