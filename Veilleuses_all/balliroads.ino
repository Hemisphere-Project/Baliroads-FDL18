#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();
int numBaliroads = 16;

// Timing
int indexOn = 0;
int timeNext = 60;
int timeOn = 600;
unsigned long TlastOn = 0;
unsigned long Tnow;
bool goOn = true;
bool balliRunning = false;

int onValue = 4000;

void balliroads_init() {
  pwm1.begin();
  pwm1.setPWMFreq(1000);
  Wire.setClock(400000);
}

void balliroads_on(int pwmPin){
  // if (id % 2) { pixels.setPixelColor(id, pixels.Color(5,5,5)); }
  // else pixels.setPixelColor(id, pixels.Color(5,0,0));
  pwm1.setPin(pwmPin, onValue);
  //Serial.printf("BALI ON: %i \n", pwmPin);
}

void balliroads_off(int pwmPin){
  // pixels.setPixelColor(id, pixels.Color(0,0,0));
  pwm1.setPin(pwmPin, 0);
  //Serial.printf("BALI OFF: %i \n", pwmPin);
}

void balliroads_stop() {
  balliRunning = false;
  for (int i =0; i<16; i++) balliroads_off(i);
}

void balliroads_start() {
  balliroads_stop();
  indexOn = 0;
  goOn = true;
  TlastOn = 0;
  balliRunning = true;
}

void balliroads_update() {
  if (!balliRunning) return;
  Tnow = millis();
  
  // ANIM TYPE 2
  // ON
  //Serial.printf("Timer: %lu, %i \n", (Tnow-TlastOn), timeNext);
  if(Tnow-TlastOn>=timeNext){
    if(goOn == true){ balliroads_on(indexOn); }
    else balliroads_off(indexOn);
    TlastOn = Tnow;
    indexOn ++;
    if(indexOn == numBaliroads){ indexOn=0; goOn = !goOn; }

  }
  
}
