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

static const uint8_t MOTOR[7] = {
  0,     /* 0: (placeholder) */
  11,    /* 1: Ton-Becher*/
  10,    /* 2: Ratsche */
  9,     /* 3: Steinschlag */
  6,     /* 4: Distelkratz */
  5,     /* 5: Flasche */
  3      /* 6: not used */
};

static const uint8_t PWM_LIMITS[7][2] = {
  {  0,   0},   /* 0 – placeholder, unused */
  { 60, 100},   /* 1 – Ton-Becher */
  {100, 255},   /* 2 – Ratsche */
  {255, 255},   /* 3 – Steinschlag */
  {255, 255},   /* 4 – Distelkratz */
  {100, 200},   /* 5 – Flasche */
  {  0,   0}    /* 6 – not used */
};

static const bool SKIP_INTRO = 1;
static const int  INTRO_DUR  = 10000; /* ms */
static const int  PAUSE_MIN  = 10000; /* ms */
static const int  PAUSE_MAX  = 30000; /* ms */
static const int  DUTY_MIN   = 5000;  /* ms */
static const int  DUTY_MAX   = 20000;  /* ms */

void intro() {
  delay(INTRO_DUR);
  for (int i = 60; i <= 255; i++) {
    analogWrite(MOTOR[4], i);
    delay(40);
  }
  delay(2000);
  analogWrite(MOTOR[4], 0);
}

void duty() {
  uint8_t i = random(1, 6);
  uint8_t j = random(1, 5);            
  if (j >= i) j++;
  uint8_t pi = random(PWM_LIMITS[i][0], PWM_LIMITS[i][1]);
  uint8_t pj = random(PWM_LIMITS[j][0], PWM_LIMITS[j][1]);
  int duri = random(DUTY_MIN, DUTY_MAX);
  int durj = random(DUTY_MIN, DUTY_MAX);
  // first object playing
  analogWrite(MOTOR[i], pi);
  delay(duri);
  analogWrite(MOTOR[i], 0);
  // second object playing
  analogWrite(MOTOR[j], pj);
  delay(durj);
  analogWrite(MOTOR[j], 0);
}

void pause() {
  int dur = random(PAUSE_MIN, PAUSE_MAX);
  delay(dur);
}

void setup() {
  for (int i; i < 7; i++) {
    pinMode(MOTOR[i], OUTPUT);
  }
  if (! SKIP_INTRO) intro(); 
}

void loop() {
  // analogWrite(MOTOR[2], 100);
  // delay(3000);
  // for (int i = 255; i >= 100; i--) {
  //   analogWrite(MOTOR[2], i);
  //   delay(20);
  // }
  // analogWrite(MOTOR[2], 100);
  // delay(1000);
  // analogWrite(MOTOR[2], 0);
  // delay(3000);

  analogWrite(MOTOR[2], 100);
  delay(3000);
  analogWrite(MOTOR[2], 0);
  analogWrite(MOTOR[4], 255);
  delay(1000);
  analogWrite(MOTOR[4], 0);


}