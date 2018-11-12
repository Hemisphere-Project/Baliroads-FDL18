
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
unsigned long Tnow;
// unsigned long timesOn[numBaliroads];
bool goOn = true;

int onValue = 4000;


// ON OFF
void baliroad_ON(int pwmPin){
  // if (id % 2) { pixels.setPixelColor(id, pixels.Color(5,5,5)); }
  // else pixels.setPixelColor(id, pixels.Color(5,0,0));
  pwm1.setPin(pwmPin, onValue);
  //Serial.printf("ON: %i \n", pwmPin);
}
void baliroad_OFF(int pwmPin){
  // pixels.setPixelColor(id, pixels.Color(0,0,0));
  pwm1.setPin(pwmPin, 0);
  //Serial.printf("OFF: %i \n", pwmPin);
}


void setup() {

  //Serial.begin(115200);
  pwm1.begin();
  pwm1.setPWMFreq(1000);
  Wire.setClock(400000);

  Tstart = millis();

}


void loop() {

  Tnow = millis();

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
  //Serial.printf("Timer: %lu, %i \n", (Tnow-TlastOn), timeNext);
  if(Tnow-TlastOn>=timeNext){
    if(goOn == true){ baliroad_ON(indexOn); }
    else baliroad_OFF(indexOn);
    TlastOn = Tnow;
    indexOn ++;
    if(indexOn == numBaliroads){ indexOn=0; goOn = !goOn; }
  }

}
