
bool state = false;

void blinker() {
  for(byte i=0; i<16; i++) {
    if (state == ((i%2)==0)) pwm1.setPin(i, 4095);
    else pwm1.setPin(i, 0);
  }
  state = !state;
}
