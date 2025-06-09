int indicatorPin = 7;
int indicatorState = LOW;
int senderPin = 11;
int sensorPin = A2;
int sensorThresh = 10;
int period = 1;
int tonePin = 2;
int toneFreq = 600;
int toneFreq2 = 800;

int sensorOnVal;
int sensorOffVal;
int sensorDelta;


void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(indicatorPin, OUTPUT);
  pinMode(senderPin, OUTPUT);
  //Serial.begin(9600);
}

void loop() {

  // we first measure sensor with IR turned off
  digitalWrite(senderPin, LOW);
  delay(period);
  sensorOffVal = analogRead(sensorPin);
  
  // then we measure sensor with IR turned on
  digitalWrite(senderPin, HIGH);
  delay(period);
  sensorOnVal = analogRead(sensorPin);

  // evaluate the result
  sensorDelta = abs(sensorOnVal - sensorOffVal);
  if (sensorDelta > sensorThresh) {
    digitalWrite(indicatorPin, LOW);
    noTone(tonePin);
  } else { 
    digitalWrite(indicatorPin, HIGH);
    tone(tonePin, toneFreq);
    delay(1000);
    tone(tonePin, toneFreq2);
    delay(1000);
  }
  //Serial.print(sensorDelta);
  //Serial.print("\n");
}
