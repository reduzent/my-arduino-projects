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


#include "NmraDcc.h"

NmraDcc  Dcc ;
DCC_MSG  Packet ;

#define This_Decoder_Address 9

extern uint8_t Decoder_Address = This_Decoder_Address;

struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};
CVPair FactoryDefaultCVs [] =
{
  {    
    CV_MULTIFUNCTION_PRIMARY_ADDRESS, This_Decoder_Address     }  
  ,
  {
    CV_ACCESSORY_DECODER_ADDRESS_MSB, 0        }
  ,
  {
    CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0        }
  ,
  {
    CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, 0        }
  ,
};

uint8_t FactoryDefaultCVIndex = 0;
void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
};

const int LED_2 = 3;

// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
//void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
void notifyDccFunc (uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{

 //Add thr accessory decoders you want to use into the sketch
  switch( FuncGrp ){
    case FN_0_4:
      digitalWrite(LED_2, !(((FuncState&0x10)>>4)));
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
  Dcc.init( MAN_ID_DIY, 10, FLAGS_MY_ADDRESS_ONLY, 0 );
  pinMode(LED_2, OUTPUT);
}

void loop()
{
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process(); 
}