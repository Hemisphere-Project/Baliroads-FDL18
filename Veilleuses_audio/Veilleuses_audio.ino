#include <Arduino.h>

// used for exemple only
unsigned long lastBip = 0;

void setup() {

  // initialize audio engine
  audio_init();

  //Serial.begin(115200);
}

void loop() {

   // must be called in every loop
   audio_update();

   // used for exemple only
   if ((millis() - lastBip) > 1000) {
    lastBip = millis();
    audio_blip();
    //Serial.println("blipping");
   }

}
