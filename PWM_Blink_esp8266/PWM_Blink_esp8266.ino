
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Ticker.h>

// PWM cards
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();

// Animator
byte anim_current = 0;
Ticker animator;

void setup() {
  pwm1.begin();
  pwm1.setPWMFreq(1000);

  Wire.setClock(400000);

  anim_select(1);
}

void loop() {
}

void anim_select(byte select) {

  animator.detach();                                // stop current animation

  if (select == 1) animator.attach(0.1, blinker);   // anim 1
  else return;                                      // No animation found

  anim_current = select;
}
