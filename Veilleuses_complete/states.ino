enum State_enum {WAIT, RECV, MORSE, BALI, PAUSE, STOP, EPILOGUE, END, AUTO};

// state feedback
const int ledPin = 10;

// state index
byte state_index = -1;
unsigned long nextStepTime = 0;
byte blinkCounter = 0;

// Callback function to states function
void (*state_function[15])();


/*
    WAIT: do nothing
*/
void state_waiting() {

  /*
      ENTER
  */
  if (state_index != WAIT) {
    LOG("SEQ: ready");
    state_index = WAIT;
    digitalWrite(ledPin, LOW);

    balliroads_stop();
    morse_stop();
    morse_wait();
  }

  /*
      EXIT
  */
  if (remote_check() == "epilogu") return state_epilogue();
  else if (remote_check() == "waiting") return state_receive();
  else if (remote_check() == "no_link") return state_auto();

  /*
      INFORM
  */
  info_waiting();
}


/*
    RECEIVE: blink bali
*/
void state_receive() {

   /*
      ENTER
  */
  if (state_index != RECV) {
    LOG("SEQ: start recv");
    state_index = RECV;
    digitalWrite(ledPin, HIGH);

    blinkCounter = 0;
    morse_stop();
  }
  

  /*
      EXIT
  */
  if (remote_check() == "working") return state_stop();
  else if (millis() >= nextStepTime) {

    if (blinkCounter < (blinkCount)*2) {
      if ((blinkCounter % 2) == 0) {
        balliroads_allON();
        nextStepTime = millis() + blinkDuration/3;
      }
      else {
        balliroads_allOFF();
        nextStepTime = millis() + 2*blinkDuration/3;
      }
      blinkCounter += 1;
    }
    else if (blinkCounter == (blinkCount)*2) {
      nextStepTime = millis() + 1000;
      blinkCounter += 1;
    }
    
    else return state_morse();
  }

  /*
      INFORM
  */
  info_working();
  
}


/*
    MORSE: start morse pharse
*/
void state_morse() {

  /*
      ENTER
  */
  if (state_index != MORSE) {
    LOG("SEQ: start morse");
    state_index = MORSE;
    digitalWrite(ledPin, HIGH);

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

    // Play MORSE
    int duration = morse_play( Phrases[activePhrase][activeWord] );
    nextStepTime = millis() + duration - balliroads_duration() * baliBefore;
  }

  /*
      EXIT
  */
  if (remote_check() == "working") return state_stop();
  else if (millis() >= nextStepTime) return state_bali();

  /*
      INFORM
  */
  info_working();
}


/*
    BALI: start bali rotation
*/
void state_bali() {

  /*
      ENTER
  */
  if (state_index != BALI) {
    LOG("SEQ: start bali");
    state_index = BALI;
    digitalWrite(ledPin, HIGH);

    balliroads_start();
    nextStepTime = millis() + baliWord * balliroads_duration();
  }

  /*
      EXIT
  */
  if (remote_check() == "working") return state_stop();
  else if (millis() >= nextStepTime) return state_pause();

  /*
      INFORM
  */
  info_working();
}


/*
    PAUSE: do nothing
*/
void state_pause() {

  /*
      ENTER
  */
  if (state_index != PAUSE) {
    LOG("SEQ: pause");
    state_index = PAUSE;
    digitalWrite(ledPin, HIGH);

    balliroads_stop();
    morse_stop();

    nextStepTime = millis() + balliroads_duration() * pauseWord;
  }

  /*
      EXIT
  */
  if (remote_check() == "working") return state_stop();
  else if (millis() >= nextStepTime) return state_stop();

  /*
      INFORM
  */
  info_working();
}


/*
    STOP: stop and wait before re-arming to prevent start collision
          or switch to epilogue if it was last word
*/
void state_stop() {

  /*
      ENTER
  */
  if (state_index != STOP) {
    LOG("SEQ: stop");
    state_index = STOP;
    if (lastWord) digitalWrite(ledPin, HIGH);
    else digitalWrite(ledPin, LOW);

    balliroads_stop();
    morse_stop();

    if (lastWord) nextStepTime = millis() + balliroads_duration(); // pre-epilogue pause
    else nextStepTime = millis() + 1000 + random(10, 500); // will not restart before 1000++ ms to prevent collision
  }

  /*
      EXIT
  */
  if (millis() >= nextStepTime) {
    if (lastWord) return state_epilogue();
    else return state_waiting();
  }

  /*
      INFORM
  */
  if (lastWord) info_working(); // still working (will go to epilogue)
  else info_waiting();
}


/*
    EPILOGUE: christmas time
*/
void state_epilogue() {

  /*
      ENTER
  */
  if (state_index != EPILOGUE) {
    LOG("SEQ: epilogue");
    state_index = EPILOGUE;
    digitalWrite(ledPin, HIGH);

    blinkCounter = 0;
  }  

  /*
      EXIT
  */
  if (millis() >= nextStepTime) {
    if (blinkCounter < 4) {

      // Premiere paire de blink
      if ((blinkCounter % 2) == 0) {
        balliroads_allON();
        morse_on(false);
        nextStepTime = millis() + (blinkDuration*1.7)/3;
      }
      else {
        balliroads_allOFF();
        morse_off();
        nextStepTime = millis() + 2*(blinkDuration*1.7)/3;
      }
      blinkCounter += 1;

      // Pause
      if (blinkCounter == 4) nextStepTime += 700;
      
    }
    else if (blinkCounter < 8) {

      // 2eme paire de blink
      if ((blinkCounter % 2) == 0) {
        balliroads_allON();
        morse_on(false);
        nextStepTime = millis() + (blinkDuration*1.7)/3;
      }
      else {
        balliroads_allOFF();
        morse_off();
        nextStepTime = millis() + 2*(blinkDuration*1.7)/3;
      }
      blinkCounter += 1;
    }
    else if (blinkCounter == 8) {

      // Pause finale
      nextStepTime = millis() + 5000;
      blinkCounter += 1;
    }
    else return state_end();
  }

  /*
      INFORM
  */
  info_epilogue();
}


/*
    EPILOGUE: christmas time
*/
void state_end() {

  /*
      ENTER
  */
  if (state_index != END) {
    LOG("SEQ: end");
    state_index = END;
    digitalWrite(ledPin, LOW);

    balliroads_stop();
    morse_stop();

    nextStepTime = millis() + balliroads_duration();
  }

  /*
      EXIT
  */
  if (millis() >= nextStepTime) {
    if (lastWord) {
      lastWord = false;
      return state_stop();
    }
    else if (remote_check() != "epilogu") return state_waiting();
  }

  /*
      INFORM
  */
  info_waiting();
}



/*
    AUTO: no link animation
*/
void state_auto() {

  /*
      ENTER
  */
  if (state_index != AUTO) {
    LOG("SEQ: auto");
    state_index = AUTO;
    digitalWrite(ledPin, LOW);

    activePhrase = 0;
    activeWord = 0;

    nextStepTime = 0;
  }

  /*
     DO STUFF
  */
  if (millis() >= nextStepTime) {
    balliroads_stop();
    int duration = morse_play("no link");
    nextStepTime = millis() + duration + 2 * balliroads_duration();
  }
  else if (!morse_isRunning() and !balliroads_isRunning()) balliroads_start();
  

  /*
      EXIT
  */
  if (remote_check() != "no_link") state_waiting();


  /*
      INFORM
  */
  info_waiting();
}


/*
    SEQUENCER
*/
void sequencer_init() {
  pinMode(ledPin, OUTPUT);

  state_function[WAIT] = state_waiting;
  state_function[RECV] = state_receive;
  state_function[MORSE] = state_morse;
  state_function[BALI] = state_bali;
  state_function[PAUSE] = state_pause;
  state_function[STOP] = state_stop;
  state_function[EPILOGUE] = state_epilogue;
  state_function[END] = state_end;
  state_function[AUTO] = state_auto;

  state_waiting();
}

void sequencer_update() {
  state_function[state_index]();
}
