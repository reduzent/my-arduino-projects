#include "Servo8Bit.h"

Servo8Bit myServo;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(1);  //attach the servo to pin PB1
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int pos = 60; pos < 120; pos++)  // goes from 0 degrees to 180 degrees
  {                                   // in steps of 1 degree
    myServo.write(pos);             // tell servo to go to position in variable 'pos'
    delay(40);                      // waits 15ms for the servo to reach the position
  }
 
  for(int pos = 120; pos > 61; pos--)  // goes from 180 degrees to 0 degrees
  {
    myServo.write(pos);             // tell servo to go to position in variable 'pos'
    delay(40);                      // waits 15ms for the servo to reach the position
  }     
}
