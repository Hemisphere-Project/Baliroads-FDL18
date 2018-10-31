
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//NodeMCU 1.0 (ESP-12E Module)
//Programmateur AVRISP MKII

// PWM cards
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();


int numBaliroads = 16;

// Timing
int indexOn = 0;
int timeNext = 60;
int timeOn = 600;
unsigned long Tstart;
unsigned long TlastOn = 0;
// unsigned long timesOn[numBaliroads];
bool goOn = true;

int onValue = 50;


// ON OFF
void baliroad_ON(int pwmPin){
  // if (id % 2) { pixels.setPixelColor(id, pixels.Color(5,5,5)); }
  // else pixels.setPixelColor(id, pixels.Color(5,0,0));
  pwm1.setPin(pwmPin, onValue);
}
void baliroad_OFF(int pwmPin){
  // pixels.setPixelColor(id, pixels.Color(0,0,0));
  pwm1.setPin(pwmPin, 0);
}


void setup() {

  // Serial.begin(9600);
  pwm1.begin();
  pwm1.setPWMFreq(1000);
  Wire.setClock(400000);

  Tstart = millis();

}


void loop() {

  unsigned long Tnow = millis();

  // ANIM TYPE 1
  // // ON
  // if(Tnow-Tstart-TlastOn>=timeNext){
  //   baliroad_ON(indexOn);
  //   timesOn[indexOn] = Tnow;
  //   TlastOn = Tnow;
  //   indexOn ++;
  //   if(indexOn == numBaliroads){ indexOn=0; }
  // }
  // // OFF
  // for(int i = 0; i < numBaliroads; i++){
  //   if(Tnow-Tstart-timesOn[i] >= timeOn){
  //     baliroad_OFF(i);
  //   }
  // }

  // ANIM TYPE 2
  // ON
  if(Tnow-Tstart-TlastOn>=timeNext){
    if(goOn == true){ baliroad_ON(indexOn); }
    else baliroad_OFF(indexOn);
    TlastOn = Tnow;
    indexOn ++;
    if(indexOn == numBaliroads){ indexOn=0; goOn = !goOn; }
  }

}
