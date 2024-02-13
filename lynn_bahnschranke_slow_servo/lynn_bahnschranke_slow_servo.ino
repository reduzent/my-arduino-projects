#include <Servo.h>

// we define a servo for our barrier
Servo schranke;

// servo end position of barrier up
float schranke_DOWN = 164.0;
// servo end position of barrier down
float schranke_UP = 62.0;
// servo current position
float schranke_POS = schranke_UP;
// servo increment
float schranke_DELTA = 0.25;
// servo target position
float schranke_TARGET;

int LEDgreen = 2;
int LEDorange = 3;
int LEDred = 4;

int SWITCH_A = 6;
int SWITCH_B = 7;

bool a = 0;
bool b = 0;

unsigned long before = millis();
unsigned long period = 5;

void setup() {
  // servo on pin 9
  schranke.attach(9);

  // set LED pins to output
  pinMode(LEDgreen, OUTPUT);
  pinMode(LEDorange, OUTPUT);
  pinMode(LEDred, OUTPUT);

  // set switch pins to INPUT_PULLUP
  // three-position switch, where center position disconnects
  // both ends
  pinMode(SWITCH_A, INPUT_PULLUP);
  pinMode(SWITCH_B, INPUT_PULLUP);
}

void loop() {
  a = digitalRead(SWITCH_A);
  b = digitalRead(SWITCH_B);
  digitalWrite(LEDgreen, !a);
  digitalWrite(LEDred, !b);
  digitalWrite(LEDorange, a && b);
  
  // execute periodically every period ms
  if ((millis() - before) >= period)
  {
    before = millis();
    // on green
    if (!a) {
      schranke_TARGET = schranke_UP;
    }
    // on red
    if (!b) {
      schranke_TARGET = schranke_DOWN;
    }
    
    if (schranke_POS > schranke_TARGET) {
      schranke_POS -= schranke_DELTA;
    }
    if (schranke_POS < schranke_TARGET) {
      schranke_POS += schranke_DELTA;
    }
    schranke_POS = constrain(schranke_POS, schranke_UP, schranke_DOWN);
    schranke.write(schranke_POS);
  }   
}
