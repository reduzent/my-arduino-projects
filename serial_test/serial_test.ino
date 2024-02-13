char x = 97;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  Serial1.begin(100000, SERIAL_8E2);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
}

void loop() {
  if (Serial1.available() > 0) {
    x = Serial1.read();
    Serial.write(x);
  }
}
