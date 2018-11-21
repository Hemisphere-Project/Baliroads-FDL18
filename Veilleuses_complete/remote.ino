#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>


WiFiUDP Udp;
unsigned int localUdpPort = 1111;
char incomingPacket[255];

unsigned long TlastUDPsent = 0;
unsigned long TlastUDPreceived = 0;

String remoteState = "no_link";

/*
 * INITIALIZE CONNECTION
 */

void remote_init() {

  String espName = "esp8266 - "+String(ESP.getChipId())+" v."+String(V_VERSION);
  WiFi.hostname(espName);

  
  LOGF("Connecting to %s ", ssid);
  WiFi.mode(WIFI_STA);
  if (password != "") WiFi.begin(ssid, password);
  else WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    LOGINL(".");
  }
  LOG(" connected");

  Udp.begin(localUdpPort);
  LOGF2("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  ArduinoOTA.setHostname( espName.c_str() );
  ArduinoOTA.begin();
  LOG("OTA started");
}


/*
 * CHECK REMOTE STATE
 */

String remote_check(){

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
      TlastUDPreceived = millis();
    }
  }

  if ((millis() - TlastUDPreceived) > 3000) remoteState = "no_link";
  
  ArduinoOTA.handle();

  return remoteState;
}

/*
 * SEND STATE
 */
 
String lastMessage;

void info_waiting() {
  info_send("waiting"); 
}

void info_working() {
  String msg = "working ";
  msg += String(activePhrase);
  msg += String(activeWord);
  info_send(msg.c_str());
  
}

void info_epilogue() {
  info_send("epilogu");
}

void info_send(const char outputMessage[]){

  bool forced = (lastMessage != String(outputMessage));
  if (forced) lastMessage = String(outputMessage);
  
  unsigned long Tnow = millis();
  if (forced or (Tnow - TlastUDPsent > infoUDPinterval)) {
    Udp.beginPacket(ipBroadcast, 1111);
    Udp.write(outputMessage);
    Udp.endPacket();
    TlastUDPsent = Tnow;
  }

}
