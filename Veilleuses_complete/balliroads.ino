#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();
int numBaliroads = 16;

// Timing
int indexOn = 0;
unsigned long TlastOn = 0;
unsigned long Tnow;
bool goOn = true;
bool balliRunning = false;

void balliroads_init() {
  pwm1.begin();
  pwm1.setPWMFreq(1000);
  Wire.setClock(400000);
}

void balliroads_on(int pwmPin){
  pwm1.setPin(pwmPin, onValue);
}

void balliroads_off(int pwmPin){
  pwm1.setPin(pwmPin, 0);
}

void balliroads_stop() {
  if (balliRunning) LOG("  balli STOP.");
  balliRunning = false;
  for (int i =0; i<16; i++) balliroads_off(i);
}

void balliroads_start() {
  balliroads_stop();
  indexOn = 0;
  goOn = true;
  TlastOn = 0;
  balliRunning = true;
  LOG("  balli START.");
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

int balliroads_duration() {
  return timeNext*32;
}

bool balliroads_isRunning() {
  return balliRunning;
}

void balliroads_allON() {
  for (int i=0; i< numBaliroads; i++) 
    balliroads_on(i);
}

void balliroads_allOFF() {
  for (int i=0; i< numBaliroads; i++) 
    balliroads_off(i);
}
