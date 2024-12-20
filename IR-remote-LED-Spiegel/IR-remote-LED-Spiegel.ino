#include <IRremote.h>

IRsend irsend;

byte packindex = 0;
byte incoming = 0;
byte packet[] = {0, 0, 0};

unsigned int IRcode = 0;
//unsigned long IRdevice = 0x00F7C03F;
unsigned long IRdevice = 0x00F70000;
int repeat = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{  
  if (Serial.available() != 0)
  {
    incoming = Serial.read();
    if((incoming >> 7) == 1) // first byte in a packet
    {
      packet[0] = incoming;
      packet[1] = 0; // reset data when init new packet
      packet[2] = 0;
      packindex = 1;
    }
    else 
    {
      packet[packindex] = incoming;
      packindex += 1;
    }
  }
  
  if(packindex >= 3)
  {
    packindex = 0;
    // | L               | M               | N               |
    // | 1 A A A A A A A | 0 A B B B B B B | 0 B B T T T T T |
    repeat = packet[2] & B00011111;
    IRcode = packet[0] << 9 | packet[1] << 2 | packet[2] >> 5;

    //// Check values
    //Serial.write(IRcode >> 8);
    //Serial.write(IRcode & 255);
    //Serial.write(repeat);
    // irsend.sendNEC(IRdevice | IRcode, repeat);
    // It seems we get only valid reads from Flipper, when repeat is set to 32
    irsend.sendNEC(IRdevice | IRcode, 32);  
    delay(100);    
  }
}