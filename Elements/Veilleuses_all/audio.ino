
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
  out->SetGain(0.1);
  
  wav = new AudioGeneratorWAV();
}

void audio_play() {
  if (wav->isRunning()) wav->stop();
  file = new AudioFileSourceSPIFFS("/sine660.wav");
  bool ok = wav->begin(file, out);
  //if (ok) LOG("Audio file started");
  //else LOG("Audio file not found");
}

void audio_stop() {
  if (wav->isRunning()) {
    wav->stop();
    //LOG("Audio file end");
  }
}

// call in main loop to refresh audio
void audio_update() {
  if (wav->isRunning()) {
      if (wav->loop()) {
        //LOG("Audio file is playing");
      }
      else audio_stop();
  }
}
