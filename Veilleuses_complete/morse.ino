
// int timeDash = 3*timeDot;
// int timePause = timeDot;
// int timeBtwLetters = 3*timeDot;
// int timeBtwWords = 7*timeDot;

//Letters
char* letters[] = {
".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", // A-I
".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R
"...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." // S-Z
};

//Numbers
char* numbers[] = {
  "-----", ".----", "..---", "...--", "....-", ".....",
"-....", "--...", "---..", "----."
};

// String phraseTest = "bonjour les amis";
// -.../---/-./.---/---/..-/.-.//+.-.././...//+.-/--/../.../
String phraseTest = "sos a";
// .../---/.../+/.-/


// Sentence Analysis
int breaksTimes[200];
bool breaksDIR[200];
int breaksSize;
int sequenceDuration;
// Sequence Loop Utils
unsigned long timeStart;
int breaksIndex = 0;
bool sequenceDone = true;
bool updateSeq = false;
bool spotIsOn = false;

void morse_init() {
  audio_init();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  morse_stop();
}

void morse_on(bool bleep){
  dmx_set(1, 255);
  dmx_update();
  digitalWrite(LED_BUILTIN, LOW);
  if (bleep) audio_play();
  spotIsOn = true;
  //LOG("DMX ON: 1");
}

void morse_on() {
  morse_on(true);
}

void morse_off(){
  dmx_set(1, 0);
  dmx_update();
  digitalWrite(LED_BUILTIN, HIGH);
  audio_stop();
  spotIsOn = false;
  //LOG("DMX OFF: 1");
}

void morse_toggle() {
  if (spotIsOn) morse_off();
  else morse_on();
}

// START
int morse_play(String phrase) {
  morse_stop();

  LOG("  morse PLAY: "+phrase);
  
  // Phrase to sequence (Letters to Morse)
  String sequence;
  for (int i = 0; i < phrase.length(); i++){
    sequence += letters[phrase[i] - 'a'];
    if(phrase[i]==' '){ sequence += "+"; }
    sequence += "/";
  }

  // Sequence code to Arrays of breakpoints
  int index=0;
  int currentTime = 0;

  for (int i = 0; i < sequence.length(); i++){
    if(sequence[i]=='.'){
      breaksTimes[index] = currentTime;
      breaksDIR[index] = HIGH;
      index++;
      currentTime += timeDot;
      breaksTimes[index] = currentTime;
      breaksDIR[index] = LOW;
      index++;
      currentTime += timeDot;
    }
    if(sequence[i]=='-'){
      breaksTimes[index] = currentTime;
      breaksDIR[index] = HIGH;
      index++;
      currentTime += 3*timeDot;
      breaksTimes[index] = currentTime;
      breaksDIR[index] = LOW;
      index++;
      currentTime += timeDot;
    }
    if(sequence[i]=='/'){
      currentTime += 2*timeDot; // 3 dots entre lettres -> 1(dot)+2(/)= 3
    }
    if(sequence[i]=='+'){
      currentTime += 2*timeDot; // 7 dots entre mots -> 1(dot)+2(/)+2(+)+2(/) = 7
    }
  }
  // Useful Vars
  breaksSize = index;
  sequenceDuration = currentTime;
  breaksIndex = 0;

  // Logs
  /*LOG("Morse ANALYSED.");
  for(int i = 0; i < breaksSize; i++){
    LOGINL("time "); LOG(breaksTimes[i]);
    LOGINL("direction "); LOG(breaksDIR[i]);
  }
  LOGINL("sequenceDuration "); LOG(sequenceDuration);
  LOG("Morse START.");*/

  timeStart = millis();
  sequenceDone = false;
  LOG("  morse START.");

  return sequenceDuration;
}

void morse_stop(){
  morse_off();
  sequenceDone = true;
  //LOG("Morse STOP.");
}

// UPDATE SEQUENCE
bool morse_update(){
  audio_update();
  dmx_update();
  
  if (sequenceDone) return false;
  
  unsigned long Tnow = millis();
  unsigned long breakTime = breaksTimes[breaksIndex] + timeStart  ;
  bool breakDIR = breaksDIR[breaksIndex];

  if((Tnow > breakTime)&&(breaksIndex<breaksSize)) {
    if(breakDIR==0) morse_off();
    else morse_on();
    breaksIndex++;
    //LOG("Morse UPDATE.");
  }
  
  // This was the last step
  if(breaksIndex == breaksSize) {
    LOG("  morse END.");
    morse_stop();
    return false;
  }

  return true;
}

bool morse_isRunning() {
  return !sequenceDone;
}
