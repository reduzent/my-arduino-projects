const int servo = 1;
int pulse;

void setup() {
  // put your setup code here, to run once:
  pinMode(servo, OUTPUT);  //attach the servo to pin PB1
}

void servo_pulse(int pin, int pos){
    int pulse = map(pos,0, 180, 850, 2050);                                  // in steps of 1 degree
    digitalWrite(pin, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(pin, LOW);
    delay(20);
}


void loop() {
  for(int pos = 0; pos <= 180; pos++)
  { 
    servo_pulse(servo, pos);
  }
 
  for(int pos = 180; pos >= 0; pos--)
  {
    servo_pulse(servo, pos);
  }     
}
