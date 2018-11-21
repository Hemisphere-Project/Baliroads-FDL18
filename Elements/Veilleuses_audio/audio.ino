
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
  out->SetGain(0.2);
  
  wav = new AudioGeneratorWAV();
}

void audio_play() {
  if (wav->isRunning()) wav->stop();
  file = new AudioFileSourceSPIFFS("/tone.wav");
  bool ok = wav->begin(file, out);
  //if (ok) Serial.println("File started");
  //else Serial.println("File not found");
}

void audio_stop() {
  if (wav->isRunning()) wav->stop();
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
