#include <avr/io.h>
#include <util/delay.h>

// Define the IR LED pin
#define IR_LED PB0

// Predefined delay values (in ms scaled by 100)
#define DELAY_SHORT_1 62  // 350 us
#define DELAY_SHORT_2 85  // 650 us
#define DELAY_LONG    155 // 1650 us
#define DELAY_END     3900 // 39.141 ms

// Raw signal data for the INPUT button (using predefined delays)
const uint16_t rawSignal[] = {
  DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_END,
  DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1, DELAY_SHORT_2, DELAY_SHORT_1, DELAY_LONG, DELAY_SHORT_1
};

// Number of pulse pairs in the raw signal
const uint16_t signalLength = sizeof(rawSignal) / sizeof(rawSignal[0]);

// Function to send the raw IR signal
void sendRawSignal() {
  for (uint16_t i = 0; i < signalLength; i++) {
    if (i % 2 == 0) {
      // Mark: Turn on the IR LED
      PORTB |= (1 << IR_LED);
    } else {
      // Space: Turn off the IR LED
      PORTB &= ~(1 << IR_LED);
    }

    // Use predefined delays
    switch (rawSignal[i]) {
      case DELAY_SHORT_1:
        _delay_ms(DELAY_SHORT_1 / 100.0);
        break;
      case DELAY_SHORT_2:
        _delay_ms(DELAY_SHORT_2 / 100.0);
        break;
      case DELAY_LONG:
        _delay_ms(DELAY_LONG / 100.0);
        break;
      case DELAY_END:
        _delay_ms(DELAY_END / 100.0);
        break;
    }
  }
  PORTB &= ~(1 << IR_LED); // Ensure the IR LED is off at the end
}

void setup() {
  // Set the IR LED pin as output
  DDRB |= (1 << IR_LED);
}

void loop() {
  // Send the raw IR signal
  sendRawSignal();
  // Wait for 5 seconds before repeating
  delay(5000);
}