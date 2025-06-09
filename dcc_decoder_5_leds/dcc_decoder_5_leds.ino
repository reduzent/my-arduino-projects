#include <EEPROM.h>
#include "NmraDcc.h"

NmraDcc  Dcc ;
DCC_MSG  Packet ;

// carriage numbering starts at 10
// each carriage has 10 accessory addresses 
// addresses 100-109 refer to functions 0-9 of carriage 10
const int carriage_no = 10;

// LED numbers correspond to actual ATTINY85 pins
// const int LED_1 = 5;  we don't use this output because it is the reset pin
// Thus, we have 4 available functions per ATTINY85
const int LED_2 = 3;  // Function 1
const int LED_3 = 4;  // Function 2
const int LED_5 = 0;  // FUnction 3
const int LED_6 = 1;  // Function 4

int state;


void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{
  if(Direction < 1){
    state = LOW;
  } else {
    state = HIGH;
  }

  switch(Addr){
    case (carriage_no*10 + 1):
      digitalWrite(LED_2, state);
      EEPROM.write(LED_2, state);
      break;
    case (carriage_no*10 + 2):
      digitalWrite(LED_3, state);
      EEPROM.write(LED_3, state);
      break;
    case (carriage_no*10 + 3):
      digitalWrite(LED_5, state);
      EEPROM.write(LED_5, state);
      break;
    case (carriage_no*10 + 4):
      digitalWrite(LED_6, state);
      EEPROM.write(LED_6, state);
      break;
      //Any address not listed above...do nothing
    default:
      break;
  }
}

void setup()
{
  Dcc.pin(0, 2, 1);

  Dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
  
  state =  EEPROM.read(LED_2);
  digitalWrite(LED_2, state); 
  state =  EEPROM.read(LED_3);
  digitalWrite(LED_3, state); 
  state =  EEPROM.read(LED_5);
  digitalWrite(LED_5, state); 
  state =  EEPROM.read(LED_6);
  digitalWrite(LED_6, state); 
}

void loop()
{
  Dcc.process(); 
}