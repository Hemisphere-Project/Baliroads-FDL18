
#include <ESPDMX.h> // https://github.com/Rickgg/ESP-Dmx

//NodeMCU 1.0 (ESP-12E Module)
//Programmateur AVRISP MKII

// DMX
DMXESPSerial dmx;

// TXD1 ESP ( = pin D4 ) <---connected--->  RXD SHIELD


void spot_ON(){
  dmx.write(1, 255);
  dmx.update();
  //Serial.printf("DMX ON: 1 \n");
}
void spot_OFF(){
  dmx.write(1, 0);
  dmx.update();
  //Serial.printf("DMX OFF: 1 \n");
}


void setup() {

  //Serial.begin(115200);

  // DMX
  dmx.init(32); // initialization for first 32 addresses by default

}


void loop() {

  spot_ON();
  delay(1000);
  spot_OFF();
  delay(1000);

}
