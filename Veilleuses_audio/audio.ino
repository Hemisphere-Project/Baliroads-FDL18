
/*
 * https://github.com/earlephilhower/ESP8266Audio
 * https://github.com/esp8266/arduino-esp8266fs-plugin
 */
#include "AudioFileSourceSPIFFS.h"
#include "AudioOutputI2S.h"
#include "AudioGeneratorWAV.h"

AudioGeneratorWAV *wav;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;

void audio_init() {
  SPIFFS.begin();
  out = new AudioOutputI2S();
  wav = new AudioGeneratorWAV();
  file = new AudioFileSourceSPIFFS("/tone.wav");
}

void audio_blip() {
  if (wav->isRunning()) wav->stop();
  file = new AudioFileSourceSPIFFS("/tone.wav");
  wav = new AudioGeneratorWAV();
  bool ok = wav->begin(file, out);
  //if (ok) Serial.println("File started");
  //else Serial.println("File not found");
}

// call in main loop to refresh audio
void audio_update() {
  if (wav->isRunning()) {
      if (wav->loop()) {
        //Serial.println("File is playing");
      }
      else {
        //Serial.println("File end");
        wav->stop();
      }
  }
}

