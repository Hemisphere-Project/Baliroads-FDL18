#define MAXCHANNEL  4   // MAX is 512

#define DMXSPEED       250000
#define DMXFORMAT      SERIAL_8N2
#define BREAKSPEED     83333
#define BREAKFORMAT    SERIAL_8N1

uint8_t dmxData[MAXCHANNEL+1] = {};

void dmx_init() {
  for (int i=0; i<=MAXCHANNEL; i++) dmxData[i] = 0;
}

void dmx_set(int chann, uint8_t value) {
  if (chann < 1) return;
  if (chann > MAXCHANNEL) return;
  if (value < 0) value = 0;
  if (value > 255) value = 255;
  
  dmxData[chann] = value;
}

void dmx_update() {

  #ifdef DEBUG
  #else
    //send data
    Serial.begin(DMXSPEED, DMXFORMAT, SERIAL_TX_ONLY);
    Serial.write(dmxData, MAXCHANNEL);
    Serial.flush();
    delay(1);
    Serial.end();
  
    //Send break
    Serial.begin(BREAKSPEED, BREAKFORMAT, SERIAL_TX_ONLY);
    Serial.write(0);
    Serial.flush();
    delay(1);
    Serial.end();
  #endif
}
