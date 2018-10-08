
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// PWM cards
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();

// Timing
unsigned long Tlast;
long OnTime, OffTime;
// Blink Type 1 - short off
long OnTimeMin1 = 2000;
long OnTimeMax1 = 3000;
long OffTimeMin1 = 50;
long OffTimeMax1 = 300;
// Blink Type 2 - long off
long OnTimeMin2 = 2000;
long OnTimeMax2 = 3000;
long OffTimeMin2 = 2000;
long OffTimeMax2 = 3000;
// Blink Type 3 - short off with double blink
long OnTimeMin3 = 2000;
long OnTimeMax3 = 3000;
long OffTimeMin3 = 200;
long OffTimeMax3 = 300;


//////////////////////////////////////////
/////////////   BLINKER    ///////////////
//////////////////////////////////////////

class Blinker
{
	int pwmPin;
	long OnTime;
	long OffTime;
	unsigned long Tlast;
  int blinkType;

  // Constructor
  public:
  Blinker(int pin)
  {
	pwmPin = pin;
	Tlast = 0;
  blinkType = 1;
  OnTime = random(OnTimeMin1,OnTimeMax1);
  OffTime = random(OffTimeMin1,OffTimeMax1);
  }

  void Update()
  {
    unsigned long Tnow = millis();
    // // ON
    // if(Tnow-Tlast>OffTime){
    //   pwm1.setPin(pwmPin, 4095);
    // }
    // // OFF
    // if(Tnow-Tlast>OffTime+OnTime){
    //   pwm1.setPin(pwmPin, 0);
    //   // RELOOP
    //   Tlast = Tnow;
    //   OnTime = random(OnTimeMin,OnTimeMax);
    //   OffTime = random(OffTimeMin,OffTimeMax);
    // }

    // ON
    if(Tnow-Tlast>OffTime){
      pwm1.setPin(pwmPin, 4095);
    }
    // OFF
    if(Tnow-Tlast>OffTime+OnTime){
      pwm1.setPin(pwmPin, 0);
      // RELOOP
      Tlast = Tnow;
      blinkType = random(1,4);
      switch (blinkType) {
        case 1:
          OnTime = random(OnTimeMin1,OnTimeMax1);
          OffTime = random(OffTimeMin1,OffTimeMax1);
          break;
        case 2:
          OnTime = random(OnTimeMin2,OnTimeMax2);
          OffTime = random(OffTimeMin2,OffTimeMax2);
          break;
        case 3:
          OnTime = random(OnTimeMin3,OnTimeMax3);
          OffTime = random(OffTimeMin3,OffTimeMax3);
          break;
      }
    }
    // BLINK TYPE 3 - During Off Time
    if((blinkType==3)&&(Tnow-Tlast<OffTime)){
      if((Tnow-Tlast>OffTime/3)&&(Tnow-Tlast<2*OffTime/3)){
        pwm1.setPin(pwmPin, 4095);
      }
      if((Tnow-Tlast>2*OffTime/3)){
        pwm1.setPin(pwmPin, 0);
      }
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
}

Blinker flotteur1(0);
Blinker flotteur2(1);
Blinker flotteur3(2);
Blinker flotteur4(3);
Blinker flotteur5(4);
Blinker flotteur6(5);
Blinker flotteur7(6);


void loop() {
  flotteur1.Update();
  flotteur2.Update();
  flotteur3.Update();
  flotteur4.Update();
  flotteur5.Update();
  flotteur6.Update();
  flotteur7.Update();
}
