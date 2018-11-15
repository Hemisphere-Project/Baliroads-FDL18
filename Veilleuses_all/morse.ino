

int timeDot = 200;
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
int timeStart;
int breaksIndex = 0;
bool sequenceDone = true;
bool updateSeq = false;
bool spotIsOn = false;

void morse_init() {
  morse_stop();
}

void morse_on(){
  dmx_set(1, 255);
  dmx_send();
  spotIsOn = true;
  //Serial.printf("DMX ON: 1 \n");
}

void morse_off(){
  dmx_set(1, 0);
  dmx_send();
  spotIsOn = false;
  //Serial.printf("DMX OFF: 1 \n");
}

void morse_toggle() {
  if (spotIsOn) morse_off();
  else morse_on();
}

// START
void morse_play(String phrase) {
  
  // Phrase to sequence (Letters to Morse)
  String sequence;
  for (int i = 0; i < phrase.length(); i++){
    sequence += letters[phrase[i] - 'a'];
    if(phrase[i]==' '){ sequence += "+"; }
    sequence += "/";
  }
  Serial.println(sequence);

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

  // Logs
  // for(int i = 0; i < breaksSize; i++){
  //   Serial.print("time "); Serial.println(breaksTimes[i]);
  //  Serial.print("direction "); Serial.println(breaksDIR[i]);
  // }
  // Serial.print("sequenceDuration "); Serial.println(sequenceDuration);

  timeStart = millis();
  sequenceDone = false;
}

void morse_stop(){
  sequenceDone = true;
}

// UPDATE SEQUENCE
void morse_update(){
  if (sequenceDone) return;
  
  unsigned long Tnow = millis();
  int breakTime = breaksTimes[breaksIndex] + timeStart  ;
  bool breakDIR = breaksDIR[breaksIndex];

  if((Tnow > breakTime)&&(breaksIndex<breaksSize)) {
    if(breakDIR==0) morse_off();
    else morse_on();
    breaksIndex++;
  }
  if(Tnow > sequenceDuration) morse_stop();
}
