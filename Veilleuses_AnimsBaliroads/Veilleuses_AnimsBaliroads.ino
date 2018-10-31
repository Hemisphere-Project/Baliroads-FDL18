
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// NEO PIXEL LIB
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define NUMBALIROADS      16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMBALIROADS, PIN, NEO_GRB + NEO_KHZ800);

// Timing
int timeNext = 60;
int timeOn = 600;
int Tstart;
int indexOn = 0;
unsigned long TlastOn = 0;
unsigned long timesOn[NUMBALIROADS];
bool goOn = true;


// ON OFF
void baliroad_ON(int id){
  // pixels.setPixelColor(id, pixels.Color(10,10,10));
  if (id % 2) { pixels.setPixelColor(id, pixels.Color(5,5,5)); }
  else pixels.setPixelColor(id, pixels.Color(5,0,0));
}
void baliroad_OFF(int id){
  pixels.setPixelColor(id, pixels.Color(0,0,0));
}


void setup() {

  Serial.begin(9600);
  pixels.begin();
  Tstart = millis();

}


void loop() {

  unsigned long Tnow = millis();
  
  // ANIM TYPE 1
  // // ON
  // if(Tnow-Tstart-TlastOn>=timeNext){
  //   baliroad_ON(indexOn);
  //   timesOn[indexOn] = Tnow;
  //   TlastOn = Tnow;
  //   indexOn ++;
  //   if(indexOn == NUMBALIROADS){ indexOn=0; }
  // }
  // // OFF
  // for(int i = 0; i < NUMBALIROADS; i++){
  //   if(Tnow-Tstart-timesOn[i] >= timeOn){
  //     baliroad_OFF(i);
  //   }
  // }

  // ANIM TYPE 2
  // ON
  if(Tnow-Tstart-TlastOn>=timeNext){
    if(goOn == true){ baliroad_ON(indexOn); }
    else baliroad_OFF(indexOn);
    TlastOn = Tnow;
    indexOn ++;
    if(indexOn == NUMBALIROADS){ indexOn=0; goOn = !goOn; }
  }

  pixels.show();
}
