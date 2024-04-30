#include <Adafruit_NeoPixel.h>
#define MONITOR_BAUDRATE 921600

#define LED_PIN 11
#define NUM_UNITS 4                                                     // 單元數量
#define NUM_LEDS_PER_UNIT 64                                            // 每個單元的LED數量
#define NUM_LEDS_TOTAL (NUM_UNITS * NUM_LEDS_PER_UNIT)                  // 總LED數量
Adafruit_NeoPixel leds(NUM_LEDS_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800);  //  定義ws2812燈條

/*------rgb變數-------*/
float client_Bright = 0.500;   //亮度
float brightIntervel = 0.04;  //亮度變化速度
int client_chang = 1;         //亮度變化方向, +-1
/*------系統變數------*/
unsigned long previousMillis = 0;
const int interval = 50;

float client_RGB[3] = { 255.00, 71.00, 34.00 };
float center_RGB[3] = { 200.00, 100.00, 20.00 };

int center_X_Y[4][2] = { {0, 7}, {0, 0}, {7, 7}, {7, 0} };

void setup() {
  /*------系統設定-------*/
  Serial.begin(MONITOR_BAUDRATE);
  leds.begin();
}

void loop() {
  /*------刷新系統變數-------*/
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    for(int unit = 0; unit < NUM_UNITS; unit++){

      for(int y= 0; y<8; y++){
        for(int x = 0 ; x < 8; x++){
          float X_Y_distence =  sqrt( sq(x * 1.00 - center_X_Y[unit][0] * 1.00 ) + sq(y * 1.00 - center_X_Y[unit][1] * 1.00));
          float bright = exp(-X_Y_distence);
          if(bright < 0.005) bright = 0.00;
          if(bright > 1.00) bright = 1.00;        
          leds.setPixelColor( (x + y*8 + unit*64), bright * client_RGB[0], bright * client_RGB[1], bright * client_RGB[2]);
          if(x == center_X_Y[unit][0]){
            if(y == center_X_Y[unit][1]){
              leds.setPixelColor(x + y*8 + unit*64, bright * center_RGB[0], bright * center_RGB[1], bright * center_RGB[2]);
            }
          }

          leds.show();
          Serial.print(x);
          Serial.print(", ");
          Serial.println(y);
          //delay(5);    
        }
      }
      leds.show();
      Serial.println("test!");
    }
  }
  Serial.println(previousMillis);
  delay(2000);
}

void refreshBright() {
  for (int i = 0; i < NUM_LEDS_PER_UNIT; i++) {
    leds.setPixelColor(i, client_Bright * client_RGB[0], client_Bright * client_RGB[1], client_Bright * client_RGB[2]);
  }
  leds.show();
}