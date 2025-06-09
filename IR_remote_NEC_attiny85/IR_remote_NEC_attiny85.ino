#include "TinyIRSender.hpp"


int key_pin = 1;

//unsigned long IRdevice = 0x00F7C03F;
unsigned long IRcode = 0x00F7C03F;
int repeat = 0;

void setup()
{
  pinMode(key_pin, INPUT_PULLUP);
}

void loop()
{
  if (!digitalRead(key_pin)) {  
    sendNEC(4, 0x5AA5, 0x0100, 8);  
    delay(1000);
  }    
}