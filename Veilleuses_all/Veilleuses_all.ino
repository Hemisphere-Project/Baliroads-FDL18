#include <Arduino.h>

// used for exemple only
unsigned long lastBip = 0;

void setup() {

  morse_init();
  audio_init();
  balliroads_init();
  
}

void loop() {

  morse_update();
  audio_update();
  balliroads_update();

  // used for exemple only
  if ((millis() - lastBip) > 4000) {
    lastBip = millis();
    morse_play("sos");
    audio_play();
    balliroads_start();
  }

}
