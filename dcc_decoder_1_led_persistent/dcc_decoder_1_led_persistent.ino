/* 11/10/2021
 *  BlankDecoderNMRAv1
 *  
 *  Based on the NmraDccAccessory_Decoder_1 example in the NMRA library examples.
 * 
 * Blank decoder that has switch statemnt set up for Accessory
 * Notice when the decoder is triggered mutliple calls are made to the decoder
 * This is not a fault, All codes are sent mutiple times very quickly
 * as if it was a loco it could be on a dirty bit of track


 Commands to send to DCC++ EX base station for testing
 <1>  = power on
 < a 25 0 > = accessory number 25 direction 0
 < a 25 1 > = accessory number 25 direction 1


 Pins used

 2 = interrupt pin for decoder messages/circuit
 


 Now with LED on Pin 3
 */

#include <EEPROM.h>
#include "NmraDcc.h"

NmraDcc  Dcc ;
DCC_MSG  Packet ;

const int LED_2 = 3;

int state;

// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{
  if(Direction < 1){
    state = LOW;
  } else {
    state = HIGH;
  }

 //Add thr accessory decoders you want to use into the sketch
  switch(Addr){
    case 12:
      digitalWrite(LED_2, state);
      EEPROM.write(0, state);
      break;
      //Any address not listed above...do nothing
    default:
      break;
  }
}

void setup()
{
  
  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
  Dcc.pin(0, 2, 1);//Pin 2 is the interrupt pin on an UNO
  
  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );
  pinMode(LED_2, OUTPUT);
  state =  EEPROM.read(0);
  digitalWrite(LED_2, state); 
}

void loop()
{
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process(); 
}