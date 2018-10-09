
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

int timeDot = 200;
int timeDash = 3*timeDot;
int timePause = timeDot;
int timeBtwLetters = 3*timeDot;
int timeBtwWords = 7*timeDot;
bool doneOnce = false;


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

// String myWord = "bonjour les amis";
// -.../---/-./.---/---/..-/.-.//+.-.././...//+.-/--/../.../
String myWord = "sos a";
// .../---/.../+/.-/


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

}

void loop() {

	if(doneOnce==false){
		doneOnce = true;
		String mySequence;
		for (int i = 0; i < myWord.length(); i++){
			mySequence += letters[myWord[i] - 'a'];
			if(myWord[i]==' '){ mySequence += "+"; }
			mySequence += "/";
		}
		Serial.println(mySequence);
		flashSequence(mySequence);
	}

}


void flashSequence(String sequence) {

	int index=0;
	int breaksTimes[200];
	bool breaksUP[200];
	int currentTime = 0;

	for (int i = 0; i < sequence.length(); i++){

		if(sequence[i]=='.'){
			breaksTimes[index] = currentTime;
			breaksUP[index] = true;
			index++;
			currentTime += timeDot;
			breaksTimes[index] = currentTime;
			breaksUP[index] = false;
			index++;
			currentTime += timeDot;
		}
		if(sequence[i]=='-'){

		}
		if(sequence[i]=='/'){

		}
		if(sequence[i]=='+'){

		}

	}
	int breaksSize = index;


	for(int i = 0; i < breaksSize; i++)
	{
	  Serial.println(breaksTimes[i]);
		Serial.println(breaksUP[i]);
	}

}




// void flashSequence(char* sequence) {
// 	int i = 0;
// 	while (sequence[i] != NULL) {
// 	        flashDotOrDash(sequence[i]);
// 	i++; }
// 	delay(timeDot * 3);
// }

// void flashDotOrDash(char dotOrDash) {
// 	digitalWrite(LED_BUILTIN, HIGH); if (dotOrDash == '.')
// 	{
// 	    delay(timeDot);
// 	  }
// 	else // must be a -
// 	{
// 	delay(timeDot * 3); }
// 	digitalWrite(LED_BUILTIN, LOW); delay(timeDot);
// }
