
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// PWM cards
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();

// Timing
unsigned long previousMillis;
long OnTimeMin = 1000;
long OnTimeMax = 4000;
long OffTimeMin = 100;
long OffTimeMax = 200;

long OnTime, OffTime;
// int ledState = LOW;

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

class Blinker
{
	int pwmPin;
	long OnTime;
	long OffTime;
	unsigned long previousMillis;

  // Constructor
  public:
  Blinker(int pin)
  {
  OnTime = random(OnTimeMin,OnTimeMax);
  OffTime = random(OffTimeMin,OffTimeMax);
	pwmPin = pin;
	previousMillis = 0;
  }

  void Update()
  {
    unsigned long currentMillis = millis();
    // OFF
    if(currentMillis-previousMillis>OffTime){
      pwm1.setPin(pwmPin, 4095);
    }
    // ON
    if(currentMillis-previousMillis>OffTime+OnTime){
      pwm1.setPin(pwmPin, 0);
      // RELOOP
      previousMillis = currentMillis;
      OnTime = random(OnTimeMin,OnTimeMax);
      OffTime = random(OffTimeMin,OffTimeMax);
    }
  }
};

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////



void setup() {
  pwm1.begin();
  pwm1.setPWMFreq(1000);
  Wire.setClock(400000);

  // OnTime = random(OnTimeMin,OnTimeMax);
  // OffTime = random(OffTimeMin,OffTimeMax);
  // previousMillis = 0;

}

Blinker flotteur1(0);
Blinker flotteur2(1);

void loop() {


  flotteur1.Update();
  flotteur2.Update();

  // unsigned long currentMillis = millis();
  // // OFF
  // if(currentMillis-previousMillis>OffTime){
  //   pwm1.setPin(1, 0);
  // }
  // // ON
  // if(currentMillis-previousMillis>OffTime+OnTime){
  //   pwm1.setPin(1, 4095);
  //   // RELOOP
  //   previousMillis = currentMillis;
  //   OnTime = random(OnTimeMin,OnTimeMax);
  //   OffTime = random(OffTimeMin,OffTimeMax);
  // }


  // unsigned long currentMillis = millis();
  // // OFF
  // if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
  // {
  //   ledState = LOW;
  //   previousMillis = currentMillis;
  //   pwm1.setPin(1, 0);
  // }
  // // ON
  // else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
  // {
  //   ledState = HIGH;
  //   previousMillis = currentMillis;
  //   pwm1.setPin(1, 4095);
  // }



}
