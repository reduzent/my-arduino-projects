int LED_0 = 0;
int LED_1 = 1;
int LED_2 = 2;
int M = 31;

void setup() {
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
}

void loop() {
  for (int i=0; i <= M; i++) {
    analogWrite(LED_0, i);
    analogWrite(LED_1, M-i);
    delay(6);
  }
  delay(400);
  for (int i=M; i >= 0; i--) {
    analogWrite(LED_0, i);
    analogWrite(LED_1, M-i);
    delay(6);
  }
  delay(400);
}
