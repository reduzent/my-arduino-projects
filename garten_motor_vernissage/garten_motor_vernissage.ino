/*
Steuerung für die Klanginstallation "Listening Interverions"
Die Installation läuft in der Pergola des Quartiergartens Hard
vom 20. September bis 29. Oktober 2026

Motorbetriebene Objekte machen Geräusche. Die Geräusche sollen
jedoch nicht durchgehend erklingen. Da die Anlage mit kleinen
Solarzellen betrieben wird, achten wir auf geringe Stromaufnahme
und betreiben daher nur einen Motor aufs Mal. 

Der Ton-Becher verwendet einen Micro-Motor mit Unwucht, der eigentlich
nur für 3V ausgelegt ist. Daher betreiben wir diesen nie mit mehr als
100 (0: off, 255: on)

Bei den anderen Motoren handelt es sich um kleine Motoren mit
Getriebe, welche für 5V ausgelegt sind. 

Bei wenig Sonnenlicht reicht der Strom, um das Arduino einzuschalten,
jedoch nicht, um einen Motor zu betreiben. Die Spannung fällt dann
soweit ab, dass das Arduino resettet wird (Brown-out). Um repetitive
Geräusche zu vermeiden, beginnt das Programm mit einem Intro, welches
eine Pause enthält. Dann lassen wir einen Motor langsam anschwellen.
Erst nachdem diese Phase überwunden wird, läuft das normale Programm.
So stellen wir sicher, dass das normale Programm nur läuft, wenn 
genug Sonnenlicht da ist. 

In der regulären Phase sollen dann jeweils zwei Objekte nacheinander
erklingen, gefolgt von einer Pause. Dann wieder zwei Objekte nacheinander, 
wobei die Objekte jeweils zufällig ausgewählt werden. So entsteht eine 
Vielzahl verschiedener Übergänge. Sowohl die Dauer der Spielzeit eines
Objektes, als auch die Pause zwischen den Gruppen sollen unterschiedlich
lang ausfallen. 


BECHER
klingt interessant mit abwechselnden Phasen von ca. 10 Sekunden
mit 80 (umegumpig) und 30 (Abschluss bouncing ball)

RATSCHE
klingt langsam wie ein Storch. Schnell klingt es recht schnell
anstrengend. Beschleunigen und Verlangsamen klingt sehr interessant.

DISTEL
PWM-Geräusche gut hörbar. Sollte darum nur voll ausgesteuert verwendet werden.

FLASCHE
Auch hier dominiert das PWM-Geräusch, wenn Motor nicht voll ausgesteuert wird.
Daher nur in Vollgas verwenden. Das Flaschengeräusch wäre sonst eh zu leise.

STEIN
Bei Vollgas braucht eine volle Umdrehung 1.6 Sekunden. Wenn man also nur einen
einzelnen Schlag erzeugen möchte, sollte der Motor genau so lange laufen.
*/

// Motorendefinition
static const uint8_t BECHER  = 1;
static const uint8_t RATSCHE = 2;
static const uint8_t STEIN   = 3;
static const uint8_t DISTEL  = 4;
static const uint8_t FLASCHE = 5;

// Dauerbereiche
static const bool SKIP_INTRO = 1;
static const long INTRO_DUR  = 180000; /* ms (3min)*/
//static const long PAUSE_MIN  = 30000;  /* ms */
//static const long PAUSE_MAX  = 300000;  /* ms */
static const long PAUSEN[] = {15000, 30000, 60000, 180000};
static const long DUTY_MIN   = 4000;   /* ms */
static const long DUTY_MAX   = 10000;  /* ms */

// Array löst Motor nach Pin auf
static const uint8_t MOTOR[7] = {
  0,     /* 0: (placeholder) */
  11,    /* 1: Ton-Becher*/
  10,    /* 2: Ratsche */
  9,     /* 3: Steinschlag */
  6,     /* 4: Distelkratz */
  5,     /* 5: Flasche */
  3      /* 6: not used */
};

static const bool MIN = 0;
static const bool MAX = 1;

// Array mit MIN und MAX Werten pro Motor
static const uint8_t PWM_LIMITS[7][2] = {
  {  0,   0},   /* 0 – placeholder, unused */
  { 60, 100},   /* 1 – Ton-Becher */
  {100, 255},   /* 2 – Ratsche */
  {100, 255},   /* 3 – Steinschlag */
  {255, 255},   /* 4 – Distelkratz */
  {255, 255},   /* 5 – Flasche */
  {  0,   0}    /* 6 – not used */
};

void play(uint8_t obj, uint8_t vel, long dur) {
  analogWrite(MOTOR[obj], vel);
  delay(dur);
  analogWrite(MOTOR[obj], 0);
}

void pause(long dur) {
  delay(dur);
}

void wait() {
  long dur = PAUSEN[random(4)];
  delay(dur);
}

void rampUp(uint8_t obj, unsigned long dur) {
  byte min = 30;
  byte max = PWM_LIMITS[obj][MAX];
  uint16_t steps = max - min;
  unsigned long t0 = millis();
  analogWrite(MOTOR[obj], min);
  for (uint16_t i = 0; i <= steps; i++) {
    unsigned long target = (dur * i) / steps;
    while (millis() - t0 < target) { /* wait */ }
    analogWrite(MOTOR[obj], min + i);
  }
  analogWrite(MOTOR[obj], 0);
}

void rampDown(uint8_t obj, unsigned long dur) {
  byte min = 30;
  byte max = PWM_LIMITS[obj][MAX];
  uint16_t steps = max - min;
  unsigned long t0 = millis();
  analogWrite(MOTOR[obj], max);
  for (uint16_t i = 0; i <= steps; i++) {
    unsigned long target = (dur * i) / steps;
    while (millis() - t0 < target) { /* spin */ }
    analogWrite(MOTOR[obj], max - i);
  }
  analogWrite(MOTOR[obj], 0);
}

void ramp(uint8_t obj, unsigned long dur, bool up) {
  byte min = 30;
  byte max = PWM_LIMITS[obj][MAX];
  uint16_t steps = max - min + 1;
  unsigned long stepDelay = dur / steps;
  if (stepDelay == 0) stepDelay = 1;
  for (uint16_t k = 0; k < steps; k++) {
    byte value = up ? (min + k) : (max - k);
    analogWrite(MOTOR[obj], value);
    delay(stepDelay);
  }
  analogWrite(MOTOR[obj], 0);
}

void intro() {
  delay(INTRO_DUR);
  rampUp(FLASCHE, 10000);
}

// "PLAYABLES" FROM HERE ////////////////////////////////////////////////////

void pickTwo() {
  uint8_t i = random(1, 6);
  uint8_t j = random(1, 5);            
  if (j >= i) j++;
  uint8_t both[2] = {i, j};
  for (uint8_t i = 0; i < 2; i++) {
    uint8_t obj = both[i];
    uint8_t vel = random(PWM_LIMITS[obj][MIN], PWM_LIMITS[obj][MAX]);
    long dur;
    if (obj == STEIN) {
      dur = 1600;
    } else {
      dur = random(DUTY_MIN, DUTY_MAX);
    }
    play(obj, vel, dur);
  }
}

void pickThree() {
  uint8_t i = random(1, 6);
  uint8_t j = random(1, 5);
  uint8_t k = random(1, 6);            
  if (j >= i) j++;
  uint8_t all[3] = {i, j, k};
  for (uint8_t i = 0; i < 3; i++) {
    uint8_t obj = all[i];
    uint8_t vel = random(PWM_LIMITS[obj][MIN], PWM_LIMITS[obj][MAX]);
    long dur;
    if (obj == STEIN) {
      dur = 1600;
    } else {
      dur = random(DUTY_MIN, DUTY_MAX);
    }
    play(obj, vel, dur);
  }
}

void ratsche() {
  play(RATSCHE, PWM_LIMITS[RATSCHE][MIN], 2000);
  ramp(RATSCHE, (2000 + (random(2) * 2000)), random(2));
  pause(500);
  ramp(RATSCHE, (2000 + (random(2) * 2000)), random(2));
  ramp(RATSCHE, (2000 + (random(2) * 2000)), random(2));
  play(RATSCHE, PWM_LIMITS[RATSCHE][MIN], 2000);
}

void flasche() {
  rampUp(FLASCHE, 500);
  play(FLASCHE, 255, (8000) + (random(3) * 8000))); // 8, 16, or 24 s
  rampDown(FLASCHE, 500);
}

void distel() {
  rampUp(DISTEL, 500);
  play(DISTEL, 255, (4000 + (random(2) * 4000)));
  rampDown(DISTEL, 500);
}

void distelPulse() {
  int rounds = random(2, 5);
  long dur = random(2) * 500 + 500;
  for (int i = 0; i <= rounds; i++) {
    play(DISTEL, 255, dur);
    pause(dur);
  }
}

void stein1x() {
  play(STEIN, 255, 1500);
}

void stein2x() {
  play(STEIN, 255, 3100);
}

void becher() {
  play(BECHER, 30, 6000);
  int rounds = random(2, 5);
  bool phase = 0;
  uint8_t vel = 0;
  for (int i = 0; i <= rounds; i++) {
    phase = !phase;
    vel = 30 + phase * 50; // either 30 or 80
    play(BECHER, vel, (2000 + random(2)*2000));
  }
}

void becher2() {
  // kürzere Sequenzen mit zusätzlichen Pausen
  int limit = random(5, 20);
  bool phase = 0;
  uint8_t vel = 0;
  for (int i = 0; i <= limit; i++) {
    phase = !phase;
    vel = phase * 50; // either 0 or 50
    play(BECHER, vel, (200 + random(4)*200));
  }
}

void distelRatsche() {
  play(DISTEL, 255, 1000);
  play(RATSCHE, 100, 3000);
  play(DISTEL, 255, 2000);
  play(RATSCHE, 255, 500);
  play(DISTEL, 255, 4000);
  rampDown(RATSCHE, 2000);
}

// "PLAYABLES" UNTIL HERE ///////////////////////////////////////////////////

void playThemAll() {
  void (*playables[])() = { 
    pickTwo, 
    pickThree, 
    ratsche,
    flasche,
    distel,
    distelPulse,
    stein1x,
    stein2x,
    becher,
    becher2,
    distelRatsche
  };
  const uint8_t NUM_ACTIONS = sizeof(playables) / sizeof(playables[0]);

  uint8_t segments = random(2, 5);   // 2, 3, or 4
  for (uint8_t s = 0; s < segments; s++) {
    uint8_t pick = random(0, NUM_ACTIONS);
    playables[pick]();
  }
  wait();
}

void setup() {
  randomSeed(analogRead(A0));
  for (int i; i < 7; i++) {
    pinMode(MOTOR[i], OUTPUT);
  } 
  if (! SKIP_INTRO) intro(); 
}

void loop() {
  playThemAll();
}