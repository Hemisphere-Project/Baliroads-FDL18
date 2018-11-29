
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// PWM cards
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();

int onValue = 4095;


// ON OFF
void baliroad_ON(int pwmPin){
  pwm1.setPin(pwmPin, onValue);
}
void baliroad_OFF(int pwmPin){
  pwm1.setPin(pwmPin, 0);
}

void setup() {

  // PWM Board
  pwm1.begin();
  pwm1.setPWMFreq(1000);
  Wire.setClock(400000);

  baliroad_ON(0);
  baliroad_ON(1);
  baliroad_ON(2);
  baliroad_ON(3);
  baliroad_ON(4);
  baliroad_ON(5);
  baliroad_ON(6);
  baliroad_ON(7);
  baliroad_ON(8);
  baliroad_ON(9);
  baliroad_ON(10);
  baliroad_ON(11);
  baliroad_ON(12);
  baliroad_ON(13);
  baliroad_ON(14);
  baliroad_ON(15);

}


void loop() {



}
