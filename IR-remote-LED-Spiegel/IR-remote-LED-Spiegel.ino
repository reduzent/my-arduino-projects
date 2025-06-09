#include <IRremote.h>

IRsend irsend;

//unsigned long IRdevice = 0x00F7C03F;
unsigned long IRcode = 0x00F7C03F;
int repeat = 0;

void setup()
{
  //
}

void loop()
{  
  irsend.sendNEC(IRcode, 32);  
  delay(10);    
}