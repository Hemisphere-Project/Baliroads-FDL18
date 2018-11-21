enum State_enum {WAIT, MORSE, BALI, PAUSE, STOP, EPILOGUE, END, AUTO};

// state feedback
const int ledPin = 10;

// state index
byte state_index = -1;
unsigned long nextStepTime = 0;

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
  }

  /*
      EXIT
  */
  if (remote_check() == "epilogu") return state_epilogue();
  else if (remote_check() == "waiting") return state_morse();
  else if (remote_check() == "no_link") return state_auto();

  /*
      INFORM
  */
  info_waiting();
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
    nextStepTime = millis() + morse_play( Phrases[activePhrase][activeWord] ) - balliroads_duration() * baliBefore;
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

    balliroads_start();
    morse_play("hhhhhhh");

    nextStepTime = millis() + baliPhrase * balliroads_duration();
  }

  /*
      EXIT
  */
  if (millis() >= nextStepTime) return state_end();

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

    morse_play("no wifi");
    balliroads_start();
  }

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
