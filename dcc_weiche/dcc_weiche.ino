#include "NmraDcc.h"
NmraDcc  Dcc ;
DCC_MSG  Packet ;

const int adresse = 25; 

const int LED = 4;
const int relay = 3;
const int servo = 1;
int pos = 90;
unsigned long last_update;
unsigned long now;

void servo_pulse(int pin, int pos){
  int pulse = map(pos,0, 180, 850, 2050);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulse);
  digitalWrite(pin, LOW);
  delay(20);
}

void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{ 
  switch(Addr){
    case adresse:
      if(Direction < 1){
        digitalWrite(LED, LOW);
        digitalWrite(relay, LOW);
        pos = 65;
      }else{
        digitalWrite(LED, HIGH);
        digitalWrite(relay, HIGH);
        pos = 115;
      }
      last_update = millis();
      break;
      default:
      break;
  }
}

void setup()
{
  Dcc.pin(0, 2, 1);
  Dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );
  pinMode(LED, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(servo, OUTPUT);
}

void loop()
{
  Dcc.process();
  now = millis();
  if ((now - last_update) < 65){
    servo_pulse(servo, pos);
  } 
}