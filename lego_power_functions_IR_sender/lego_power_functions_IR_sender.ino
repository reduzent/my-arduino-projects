const byte ledMask = B00100000; // Pin 13 (right most is Pin 8)

const unsigned int PULSE = 158;  // microseconds
const unsigned int LPAUS = 263;  // Pause (Low)
const unsigned int HPAUS = 533;  // Pause (High)
const unsigned int SPAUS = 1026; // Pause (Start & Stop bit)

/*
Format of control byte received from serial
| X X C C | B B  R R |
X = not used
C = Channel (0-3)
B = Direction (blue)
R = Direction (red) 
 */
byte rx = 0; 

/*
| T E C C | a M M M | D D D D | L L L L |
T = Toggle
E = Escape
C = Channel
a = Address
M = Mode
D = Data
L = LRC (Checksum)
*/
unsigned int message = (B10001001 << 8) | B01110100;

unsigned int compose_message(byte rx) {
  /*
  Composes 4-Nibble power functions message
  from given input 
   */
   byte nibble1 = B00001000 | (( rx & 48) >> 4);
   byte nibble2 = B00000001;
   byte nibble3 = rx & 15;
   byte nibble4 = B00001111 ^ nibble1 ^ nibble2 ^ nibble3;
   unsigned int message =   
        ((nibble1 & 15) << 12)
      | ((nibble2 & 15) << 8) 
      | ((nibble3 & 15) << 4) 
      | (nibble4 & 15);
   return message; 
}


void single_blink(byte type) {
  // type:
  // low = 0
  // high = 1
  // startstop = 2
  // Set Pin to HIGH
  PORTB = PORTB | ledMask;
  delayMicroseconds(PULSE);
  PORTB = ~PORTB | ~ledMask; 
  switch(type) {
   case 0: delayMicroseconds(LPAUS); break;
   case 1: delayMicroseconds(HPAUS); break;
   case 2: delayMicroseconds(SPAUS); break;
  }
}

void setup() {
  // Set pin to OUTPUT
  DDRB = DDRB | ledMask;
  Serial.begin(9600);
}

void loop() {
  
  // only proceed if we actually receive something
  while(Serial.available() == 0) {
    delayMicroseconds(1);
  }

  // skip to the last received byte
  while(Serial.available()) {
    rx = Serial.read();
  }

  message = compose_message(rx);
  Serial.write(message>>8);
  Serial.write(message);
  
  // Startbit
  single_blink(2);

  // message
  for (int i = 0; i < 16; i++) {
    single_blink((bit(i) & message) >> i);
  }

  // Stopbit
  single_blink(2);
}
