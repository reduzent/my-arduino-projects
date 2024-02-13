void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  for(int i = 0; i < 100; i++ ) {
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(1);                      
    digitalWrite(LED_BUILTIN, LOW);    
    delay(19);
  }
  for(int i = 0; i < 100; i++ ) {
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(2);                      
    digitalWrite(LED_BUILTIN, LOW);    
    delay(18);
  }
}
