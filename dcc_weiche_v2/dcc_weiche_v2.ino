#include "NmraDcc.h"
NmraDcc  Dcc ;
DCC_MSG  Packet ;

const int adresse = 25; 

const int LED = 4;
const int relay = 3;
const int servo = 1;
const int pos_straight = 93;
const int pos_turn = 156;

bool state = false;
bool got_packet = true;

void servo_pulse(int pin, int pos){
  int pulse = map(pos,0, 180, 850, 2050);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulse);
  digitalWrite(pin, LOW);
  delay(21);
}

void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{ 
  switch(Addr){
    case adresse:
      if(Direction < 1){
        state = false;
      }else{
        state = true;
      }
      got_packet = true;
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
  if ( got_packet ) {
    detachInterrupt(0);
    if (state) {
      for (int i = pos_straight; i <= pos_turn; i++) {
        servo_pulse(servo, i);
      }
      digitalWrite(LED, HIGH);
      digitalWrite(relay, HIGH);
      state = false;
    } else {
      for (int i = pos_turn; i >= pos_straight; i--) {
        servo_pulse(servo, i);
      }
      digitalWrite(LED, LOW);
      digitalWrite(relay, LOW);
      state = true;
    }
    got_packet = false;
    Dcc.pin(0,2,1);
    Dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );
  }
 
}