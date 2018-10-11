
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// lib
#include <WiFi.h>
#include <WiFiUdp.h>

const int MTUu = 1472;  // Usable MTU (1500 - 20 IP - 8 UDP)
unsigned char incomingPacket[MTUu];  // buffer for incoming packets

// Timing
unsigned long timeOn;

void setup() {

  Serial.begin(9600);
  wifi_init();

}


void loop() {

  // UPDATE with data received
  if ( wifi_read(incomingPacket) ){
    // Serial.println( incomingPacket );
  }


  //

}
