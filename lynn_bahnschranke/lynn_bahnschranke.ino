#include <Servo.h>

Servo schranke;

// servo position of barrier up
int schranke_DOWN = 164;
// servo position of barrier down
int schranke_UP = 62;
// 
int schranke_POS;

int LEDgreen = 2;
int LEDorange = 3;
int LEDred = 4;

int SWITCH_A = 6;
int SWITCH_B = 7;

bool a = 0;
bool b = 0;

unsigned long before = millis();

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
  
  // execute periodically every 50 ms
  if ((millis() - before) >= (unsigned long)50)
  {
    before = millis();

    // on green
    if (!a) {
      schranke.write(schranke_oben);
    }

    // bei rot
    if (!b) {
      schranke.write(schranke_unten);
    }
  } 
}
