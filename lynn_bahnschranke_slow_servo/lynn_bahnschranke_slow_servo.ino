
// servo end position of barrier up
const float schranke_DOWN = 185.0;
// servo end position of barrier down
const float schranke_UP = 66.0;
// servo current position
float schranke_POS = schranke_UP;
// servo increment
const float schranke_DELTA = 1;
// servo target position
float schranke_TARGET;

int servo = 1;

bool pulse_on = false;

int LEDgreen = 3;
int LEDorange = 2;
int LEDred = 4;

int SWITCH_A = 5;
int SWITCH_B = 0;

bool a = 0;
bool b = 0;

void servo_pulse(int pin, int pos){
  int pulse = map(pos,0, 180, 700, 2300);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulse);
  digitalWrite(pin, LOW);
  delay(20);
}

void setup() {
  // set servo to OUTPUT
  pinMode(servo, OUTPUT);

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
    pulse_on = true;
  } else if (schranke_POS < schranke_TARGET) {
    schranke_POS += schranke_DELTA;
    pulse_on = true;
  } else if (schranke_POS == schranke_TARGET) {
    pulse_on = false;
  }
  if (pulse_on) {
    servo_pulse(servo, schranke_POS);    
  }

   
}
