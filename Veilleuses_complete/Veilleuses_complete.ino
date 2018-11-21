#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "debug.h"

#define V_VERSION 1.0   // first release 21/11/2018 with OTA


/*
   CONFIG
*/

// WIFI
const char* ssid = "101WIFI";
const char* password = "101internet";       // "" if no password
IPAddress ipBroadcast(192, 168, 0, 255);    

// MORSE
int timeDot = 30; //100
float volumeDot = 0.7;

// PWM
int timeNext = 30;  // 60
int onValue = 200;  //4095

// SEQ
int pauseWord = 0; // pause after a word
float baliBefore = 0.5; // number of bali before word ends
int baliWord = 1; // number of bali per word
int baliPhrase = 1; // number of bali at phrase end



/*
   PHRASES
*/
#define N_PHRASE 2
String Phrases[N_PHRASE][10] = {
  {"peut", "etre", "ultime", "enigme", "ou", "derniere", "illusion"},
  {"comme", "ces", "reflets", "rouges", "le", "soir", "a", "lhorizon"}
};
int PhrasesSizes[N_PHRASE] = {7, 8};

int activePhrase = 0;
int activeWord = 0;
int remotePhrase = 0;
int remoteWord = -1;
bool lastWord = false;

/*
    SETUP
*/
void setup() {

  LOGSETUP();
  LOG("Started.");

  // init engines
  morse_init();
  balliroads_init();

  // connect to remote
  remote_init();

  // init state machine sequencer
  sequencer_init();

}

/*
    LOOP
*/
void loop() {

  // get remote status
  remote_check();

  // run sequencer
  sequencer_update();

  // update engines
  morse_update();
  balliroads_update();

}
