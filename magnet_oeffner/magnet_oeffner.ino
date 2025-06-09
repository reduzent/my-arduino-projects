#include "Servo8Bit.h"

Servo8Bit myservo;

int servo_pin = 0;
int reed_pin = 2;
int open_pos = 80;
int close_pos = 43;  
int holdtime = 5000;
 
void setup() {
  myservo.attach(servo_pin); 
  pinMode(reed_pin, INPUT_PULLUP);
  myservo.write(close_pos);
  delay(500);
  myservo.detach();
}

void loop() {
  if  (!digitalRead(reed_pin))
  {
    myservo.attach(servo_pin);
    myservo.write(open_pos);
    delay(500);
    myservo.detach();
    delay(holdtime);
    myservo.attach(servo_pin);
    myservo.write(close_pos);
    delay(1000);
    myservo.detach();
  }
}