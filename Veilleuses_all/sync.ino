#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "101WIFI";
const char* password = "101internet";
IPAddress ipBroadcast(192, 168, 0, 255);

WiFiUDP Udp;
unsigned int localUdpPort = 1111;
char incomingPacket[255];

int UDPinterval = 200;
unsigned long TlastUDP = 0;

const int ledPin = 10;
String remoteState = "no_link";

/*
 * INITIALIZE CONNECTION
 */

void sync_init() {
  pinMode(ledPin, OUTPUT);

  LOGF("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    LOGINL(".");
  }
  LOG(" connected");

  Udp.begin(localUdpPort);
  LOGF2("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


/*
 * CHECK REMOTE STATE
 */

String sync_checkRemote(){

  String udpmsg = "";
  int packetSize = Udp.parsePacket();
  int len = 0;
  if (packetSize){
    int len = Udp.read(incomingPacket, 255);
    if (Udp.remoteIP() == WiFi.localIP()) len = 0;
    incomingPacket[len] = 0; //
    Udp.flush();
    if (len > 0) {
      String income = String(incomingPacket);
      remoteState = income.substring(0,7);
      if (remoteState == "working") {
        remotePhrase = income.substring(8,9).toInt();
        remoteWord = income.substring(9,10).toInt();
      }
    }
  }
 
  return remoteState;
}

/*
 * SEND STATE
 */

void sync_waiting() {
  sync_send("waiting");
  digitalWrite(ledPin, LOW);
}

void sync_working(int phrase, int mot) {
  String msg = "working ";
  msg += String(phrase);
  msg += String(mot);
  sync_send(msg.c_str());
  digitalWrite(ledPin, HIGH);
}

void sync_epilogu() {
  sync_send("epilogu");
  digitalWrite(ledPin, HIGH);
}

void sync_send(const char outputMessage[]){

  unsigned long Tnow = millis();
  if (Tnow - TlastUDP > UDPinterval) {
    Udp.beginPacket(ipBroadcast, 1111);
    Udp.write(outputMessage);
    Udp.endPacket();
    TlastUDP = Tnow;
  }

}
