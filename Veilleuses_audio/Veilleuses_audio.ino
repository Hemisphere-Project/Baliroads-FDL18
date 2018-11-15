#include <Arduino.h>

// used for exemple only
unsigned long lastBip = 0;

void setup() {
  //Serial.begin(115200);
  
  audio_init();
}

void loop() {

   audio_update();

   // used for exemple only
   if ((millis() - lastBip) > 1000) {
    lastBip = millis();
    audio_play();
    //Serial.println("blipping");
   }

}
