
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "101WIFI";
const char* password = "101internet";

WiFiUDP Udp;
unsigned int localUdpPort = 1111;
char incomingPacket[255];
IPAddress ipBroadcast(192, 168, 0, 255);
//utils
int UDPinterval = 100;
unsigned long TlastUDP = 0;

const int ledPin = LED_BUILTIN;
unsigned long Trestart;
unsigned long T_ON = 4000;

bool readyToWork = true;

void setup() {

  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  Trestart = millis();

}


void loop() {

  unsigned long Tnow = millis();

  String udpmsg = wifi_listen();
  // if (udpmsg != "") { Serial.println(udpmsg); }

  //LISTEN
  if (udpmsg == "your_turn" && readyToWork){
    Trestart = Tnow;
    readyToWork = false;
  }
  else if (udpmsg == "im_working") {
    readyToWork = true;
  }

  // IT'S MY TURN
  if (Tnow - Trestart < T_ON) {
    digitalWrite(ledPin, HIGH);
    wifi_send("im_working");
    // Start job here
  }

  // I'VE FINISHED
  if (Tnow - Trestart > T_ON) {
    digitalWrite(ledPin, LOW);
    wifi_send("your_turn");
  }



}

String wifi_listen(){

  int packetSize = Udp.parsePacket();
  int len = 0;
  if (packetSize){
    int len = Udp.read(incomingPacket, 255);
    if (Udp.remoteIP() == WiFi.localIP()) len = 0;
    incomingPacket[len] = 0; //
    Udp.flush();
    if (len > 0) return String(incomingPacket);
  }
  return "";

}

void wifi_send(char outputMessage[]){

  unsigned long Tnow = millis();
  if (Tnow - TlastUDP > UDPinterval) {
    Udp.beginPacket(ipBroadcast, 1111);
    Udp.write(outputMessage);
    Udp.endPacket();
    TlastUDP = Tnow;
  }

}
