#include <Arduino.h>
#include "debug.h"


/*
   CONFIG
*/

// MORSE
int timeDot = 50; //100

// PWM
int timeNext = 60;  // 60
int onValue = 1024;  //4096

// SEQ
int pauseWord = 0; // pause after a word
float baliBefore = 0.5; // number of bali before word ends
int baliWord = 2; // number of bali per word
int baliPhrase = 3; // number of bali at phrase end

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
bool isSlaveEpiloguing = false;

byte current_step = 0;
unsigned long nextStepTime = 0;

/*
    SETUP
*/
void setup() {

  LOGSETUP();
  LOG("Started.");

  // connect to nework
  sync_init();

  // init engines
  morse_init();
  balliroads_init();

}

/*
    LOOP
*/
void loop() {

  // remote is working: i should be waiting
  //
  if ( sync_checkRemote() == "working" && current_step != 5 ) {

    // stop me if i was running too
    if (current_step > 0) stop_sequence();
    isSlaveEpiloguing = false;
  }

  // remote is epiloguing: do it too
  else if ( sync_checkRemote() == "epilogu" && current_step == 0 && !isSlaveEpiloguing) {
    play_step(5);
    isSlaveEpiloguing = true;
  }

  // remote is waiting or epiloguing: i should be working
  //
  else {

    // go to next step
    if (millis() > nextStepTime) next_step();

  }

  // update engines
  morse_update();
  balliroads_update();

  // send state
  if (current_step == 0) sync_waiting();
  else if (current_step == 5 && lastWord) sync_epilogu();
  else sync_working(activePhrase, activeWord);

}


/*
    NEXT STEP
*/
void next_step() {
  play_step(current_step + 1);
}

/*
    PLAY STEP
*/
void play_step(int st) {
  current_step = st;

  switch (current_step) {

    // morse
    case 1:

      // next word
      activePhrase = remotePhrase;
      activeWord = remoteWord + 1;

      // playing last word
      lastWord = (activeWord + 1 == PhrasesSizes[activePhrase]);

      // next phrase
      if (activeWord >= PhrasesSizes[activePhrase]) {
        activeWord = 0;
        activePhrase++;
        if (activePhrase >= N_PHRASE) activePhrase = 0;
      }

      LOG(Phrases[activePhrase][activeWord]);
      nextStepTime = millis() + morse_play( Phrases[activePhrase][activeWord] ) - balliroads_duration() * baliBefore;
      LOG("SEQ: start morse");
      break;

    // bali
    case 2:
      balliroads_start();
      nextStepTime = millis() + baliWord * balliroads_duration();
      LOG("SEQ: start bali");
      break;

    // pause
    case 3:
      balliroads_stop();
      morse_stop();
      nextStepTime = millis() + balliroads_duration() * pauseWord;
      LOG("SEQ: pause");
      break;

    // stop
    case 4:
      current_step = 0;
      balliroads_stop();
      morse_stop();
      nextStepTime = millis() + 1000; // will not restart before 1000 ms to prevent collision

      // epilogue
      if (lastWord) {
        current_step = 4;
        nextStepTime = millis() + balliroads_duration();
      }
      break;

    // phrase end
    case 5:
      sync_epilogu();
      balliroads_start();
      morse_play("hhhhhhh");
      nextStepTime = millis() + baliPhrase * balliroads_duration();
      LOG("EPILOGUING");
      break;

    // stop but ready to restart
    case 6:
      current_step = 0;
      balliroads_stop();
      morse_stop();
      nextStepTime = millis() + balliroads_duration();
      if (lastWord) {
        lastWord = false;
        nextStepTime += 1000;
      }

      break;

  }
}

void stop_sequence() {
  play_step(4);
}
